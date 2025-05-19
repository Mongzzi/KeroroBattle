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
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));

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
	AttackRange = 200.0f;
	AttackRadius = 50.0f;
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


	// 스탯컴포넌트 체력0 델리게이트 바인딩
	EnemyStat->OnHpIsZero.AddLambda([this]()->void {
		EnemyAnim->SetDeadAnim();
		SetActorEnableCollision(false);
		});
}

float AKeroroEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (bIsDead || EnemyStat == nullptr)
		return 0.0f;

	// 받은 데미지만큼 체력 감소
	EnemyStat->SetDamage(DamageAmount);

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

	// 일정 시간 후 소멸
	SetLifeSpan(2.0f);

	// 굳이 EventInstigator 사용하지않고 GameplayStatics 함수로 사용했음 // ai컨트롤러가 처치시 코드가 복잡해짐 추후 멀티플레이 추가시 수정해야할듯
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		if (AKeroroPlayerState* PS = Cast<AKeroroPlayerState>(PC->PlayerState))
		{
			PS->AddKillEnemyNum();

			// exp 오브젝트 생성
			AExpObject* ExpObj = GetWorld()->SpawnActor<AExpObject>(AExpObject::StaticClass(), GetActorLocation(), FRotator::ZeroRotator);
			if (ExpObj)
			{
				AKeroroCharacter* KR = Cast<AKeroroCharacter>(PC->GetCharacter());
				int32 DropExp = EnemyStat->GetDropExp();
				// 여기서 타겟 , 경험치구슬 속도 , 경험치 수치 정해주고 구슬이 캐릭터에 닿으면 addexp호출
				ExpObj->SetTargetAndSpeedAndExp(KR, 1.5f, DropExp);
			}

			// 골드 오브젝트 생성
			ADropGold* GoldObj = GetWorld()->SpawnActor<ADropGold>(ADropGold::StaticClass(), GetActorLocation(), FRotator::ZeroRotator);
		}
	}
}

void AKeroroEnemyCharacter::AttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this); // 자기 자신은 무시

	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),	// 시작위치
		GetActorLocation() + GetActorForwardVector() * AttackRange,	// 끝위치
		FQuat::Identity, // 회전 없음
		ECC_GameTraceChannel3, // Attack채널 ( DefaultEngine 파일에 내가만든 채널 몇번쨰인지 나와있음 )
		FCollisionShape::MakeSphere(AttackRadius),
		Params
	);

#if ENABLE_DRAW_DEBUG
	FVector TraceVec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bHit ? FColor::Green : FColor::Red;
	float DebugLifeTime = 5.0f;

	//DrawDebugCapsule(
	//	GetWorld(),
	//	Center,
	//	HalfHeight,
	//	AttackRadius,
	//	CapsuleRot,
	//	DrawColor,
	//	false,
	//	DebugLifeTime
	//);

#endif

	if (bHit)
	{
		// 캐스트 성공시 참반환 적캐릭터만 데미지주게 
		if (IsValid(HitResult.GetActor()) && Cast<AKeroroCharacter>(HitResult.GetActor()))
		{
			FDamageEvent DamageEvent;
			HitResult.GetActor()->TakeDamage(EnemyStat->AttackPower * 2, DamageEvent, GetController(), this);
			//UE_LOG(LogTemp, Warning, TEXT(" hitted : %s"), *HitResult.GetActor()->GetName());

		}
	}
}