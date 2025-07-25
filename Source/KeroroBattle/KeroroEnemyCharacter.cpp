// Fill out your copyright notice in the Description page of Project Settings.


#include "KeroroEnemyCharacter.h"
#include "KeroroCharacter.h"
#include "KeroroStatComponent.h"
#include "KeroroPlayerController.h"
#include "KeroroPlayerState.h"
#include "KeroroAIController.h"
#include "EnemyAIController.h"
#include "KeroroAnimInstance.h"
#include "KeroroHPBarWidget.h"
#include "ExpObject.h"
#include "DropGold.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AKeroroEnemyCharacter::AKeroroEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	EnemyStat = CreateDefaultSubobject<UKeroroStatComponent>(TEXT("KRSTAT"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SKMESH(TEXT("/Game/Keroro_Model/joriri/joriri.joriri"));
	if (SKMESH.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SKMESH.Object);
	}
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("EnemyCharacter"));

	GetCharacterMovement()->RotationRate = FRotator(0.0f, 600.0f, 0.0f);
	GetCharacterMovement()->bOrientRotationToMovement = true;	// 이동 방향으로 자동 회전
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;

	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	static ConstructorHelpers::FClassFinder<UAnimInstance>ANIM(TEXT("/Game/Blueprints/KR_AnimInstance.KR_AnimInstance_C"));
	if (ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(ANIM.Class);
	}

	// HP바 추가
	HPBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));
	HPBar->SetupAttachment(GetMesh());
	HPBar->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	HPBar->SetWidgetSpace(EWidgetSpace::Screen);
	HPBar->SetDrawSize(FVector2D(300.0f, 50.0f));
	static ConstructorHelpers::FClassFinder<UUserWidget>HUD(TEXT("/Game/Blueprints/KR_HPBar.KR_HPBar_C"));
	if (HUD.Succeeded()) HPBar->SetWidgetClass(HUD.Class);

	bIsDead = false;
	AttackRange = 400.0f;
	AttackRadius = 25.0f;
}

void AKeroroEnemyCharacter::Attack()
{
	if (!EnemyAnim) return;
	EnemyAnim->PlayAttackMontage();
}

// Called when the game starts or when spawned
void AKeroroEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	// HP바 위젯
	auto HpBarWidget = Cast<UKeroroHPBarWidget>(HPBar->GetUserWidgetObject());
	if (HpBarWidget != nullptr)
	{
		HpBarWidget->BindKRStat(EnemyStat);
	}
	if (EnemyStat)
	{
		EnemyStat->SetLevel(1);
	}
}

// Called every frame
void AKeroroEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKeroroEnemyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	EnemyAnim = Cast<UKeroroAnimInstance>(GetMesh()->GetAnimInstance());
	EnemyAnim->OnAttackHitCheck.AddUObject(this, &AKeroroEnemyCharacter::AttackCheck);


	//// 스탯컴포넌트 체력0 델리게이트 바인딩
	//EnemyStat->OnHpIsZero.AddLambda([this]()->void {
	//	EnemyAnim->SetDeadAnim();
	//	SetActorEnableCollision(false);
	//	});
}

float AKeroroEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (bIsDead || EnemyStat == nullptr)
		return 0.0f;


	float RandEvasion = FMath::FRand();
	//UE_LOG(LogTemp, Error, TEXT("evation rate = %f, "), EnemyStat->EvasionRate);

	if (RandEvasion < EnemyStat->EvasionRate) {
		UE_LOG(LogTemp, Error, TEXT("evasion attack~~"));
		return 0.0f;
	}

	float FinalDamage = DamageAmount * (1.0f - EnemyStat->DefenseRate);
	//UE_LOG(LogTemp, Error, TEXT("in damage = %f, final Damage = %f"), DamageAmount, FinalDamage);

	// 받은 데미지만큼 체력 감소
	EnemyStat->SetDamage(FinalDamage);

	// 체력이 0이하가 되면 die함수 호출
	if (EnemyStat->GetHpRatio() <= 0.0f)
	{
		Die();
	}
	return DamageAmount;
}

void AKeroroEnemyCharacter::Die()
{
	if (bIsDead) return;

	bIsDead = true;

	// AI 컨트롤러가 있으면 언포제스
	AEnemyAIController* AC = Cast<AEnemyAIController>(GetController());
	if (AC)
	{
		AC->UnPossess();
	}
	// 콜리전 끄기
	SetActorEnableCollision(false);

	// 애니메이션 재생
	if(EnemyAnim) EnemyAnim->SetDeadAnim();

	// 일정 시간 후 소멸
	SetLifeSpan(1.5f);


	// 굳이 EventInstigator 사용하지않고 GameplayStatics 함수로 사용했음 // ai컨트롤러가 처치시 코드가 복잡해짐 추후 멀티플레이 추가시 수정해야할듯
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		if (AKeroroPlayerState* PS = Cast<AKeroroPlayerState>(PC->PlayerState))
		{
			PS->AddKillEnemyNum();

			AKeroroCharacter* KR = Cast<AKeroroCharacter>(PC->GetCharacter());
			// 처치시 Mp,Hp 회복
			if (KR->KRStat)
			{
				KR->KRStat->AttackMpHeal();
				KR->KRStat->AttackHeal();
			}

			// exp 오브젝트 생성
			AExpObject* ExpObj = GetWorld()->SpawnActor<AExpObject>(AExpObject::StaticClass(), GetActorLocation(), FRotator::ZeroRotator);
			if (ExpObj)
			{
				int32 DropExp = EnemyStat->GetDropExp();
				int32 FinalDropExp = static_cast<int32>(DropExp * PS->ExpGainRate);

				// 여기서 타겟 , 경험치구슬 속도 , 경험치 수치 정해주고 구슬이 캐릭터에 닿으면 addexp호출
				ExpObj->SetTargetAndSpeedAndExp(KR, 1.5f, FinalDropExp);
			}

			// 골드 오브젝트 생성
			FTransform SpawnTransform = FTransform(FRotator::ZeroRotator, GetActorLocation());
			ADropGold* GoldObj = GetWorld()->SpawnActorDeferred<ADropGold>(ADropGold::StaticClass(), SpawnTransform);
			GoldObj->SetGoldMount(EnemyStat->GetDropGold());
			UGameplayStatics::FinishSpawningActor(GoldObj, SpawnTransform);
		}
	}
}

void AKeroroEnemyCharacter::AttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity,
		ECC_GameTraceChannel3,
		FCollisionShape::MakeSphere(AttackRadius),
		Params
	);

	if (bHit)
	{
		// 캐스트 성공시 참반환 적캐릭터만 데미지주게 
		if (IsValid(HitResult.GetActor()) && Cast<AKeroroCharacter>(HitResult.GetActor()))
		{
			FDamageEvent DamageEvent;

			float FinalDamage = EnemyStat->AttackPower;
			float Rand = FMath::FRand();
			if (Rand < EnemyStat->CritChanceRate)
			{
				FinalDamage *= EnemyStat->CritDamageRate;
				UE_LOG(LogTemp, Error, TEXT("Critical~~~ Damage = %f // Default Damage = %f /// CriticalDamage Rate = %f"), FinalDamage, EnemyStat->AttackPower, EnemyStat->CritDamageRate);
			}

			HitResult.GetActor()->TakeDamage(FinalDamage, DamageEvent, GetController(), this);

		}
	}
}