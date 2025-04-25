// Fill out your copyright notice in the Description page of Project Settings.


#include "KeroroCharacter.h"
#include "KeroroPlayerController.h"
#include "KeroroAnimInstance.h"
#include "KeroroWeapon.h"
#include "KeroroPlayerState.h"
#include "KeroroStatComponent.h"
#include "KeroroHPBarWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "DrawDebugHelpers.h"	// 디버그 드로잉 기능 사용하기위한 헤더
#include "Engine/DamageEvents.h"


// Sets default values
AKeroroCharacter::AKeroroCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 메시 위치 조정
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));

	// 카메라 스프링암
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 50.0f, 60.0f), FRotator(-15.0f, 0.0f, 0.0f));

	// 캡슐컴포넌트 콜리전프로파일 설정 
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("KeroroCharacter"));

	// 캐릭터 스탯 컴포넌트
	KRStat = CreateDefaultSubobject<UKeroroStatComponent>(TEXT("KRSTAT"));

	// 나이아가라 이펙트 추가
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NE(TEXT("/Game/Knife_light/VFX/NE_attack02.NE_attack02"));
	if (NE.Succeeded())
	{
		NSAttackEffect = NE.Object;
	}

	// HP바 추가
	HPBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));
	HPBar->SetupAttachment(GetMesh());
	HPBar->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	HPBar->SetWidgetSpace(EWidgetSpace::Screen);
	HPBar->SetDrawSize(FVector2D(300.0f, 50.0f));
	static ConstructorHelpers::FClassFinder<UUserWidget>HUD(TEXT("/Game/Blueprints/KR_HPBar.KR_HPBar_C"));
	if (HUD.Succeeded()) HPBar->SetWidgetClass(HUD.Class);

	// 스프링암 설정
	SpringArm->bUsePawnControlRotation = true; // 컨트롤러 기준 회전
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = true; // Yaw만 따라가게
	SpringArm->bDoCollisionTest = true; // 벽에 가까이 갔을 때 카메라 충돌 보정

	// 카메라 설정
	Camera->bUsePawnControlRotation = false;

	// 캐릭터 회전 관련 설정
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true; // 입력 방향 따라 몸 회전
	GetCharacterMovement()->bUseControllerDesiredRotation = false; // 컨트롤러 회전 비활성화

	// 캐릭터 속도
	WalkSpeed = 600.0f;
	RunSpeed = 1200.0f;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	// 공격 체크 - 추후 수정필요 상속받을 클래스에 넣어야함 // 무기마다 다르게
	IsAttacking = false;
	MaxCombo = 4;
	AttackEndComboState();

	// 공격 범위
	AttackRange = 200.0f;
	AttackRadius = 50.0f;
}

void AKeroroCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// (스켈레탈메시,애님인스턴스 로드 후 설정),(몽타주 델리게이트 바인딩)
	LoadAssetandSetting(CurrentKeroroType);

	// 스탯컴포넌트 체력0 델리게이트 바인딩
	KRStat->OnHpIsZero.AddLambda([this]()->void {
		KRAnim->SetDeadAnim();
		SetActorEnableCollision(false);
		});
}

// Called when the game starts or when spawned
void AKeroroCharacter::BeginPlay()
{
	Super::BeginPlay();
	FName WeaponSocket(TEXT("hand_rSocket"));
	auto CurWeapon = GetWorld()->SpawnActor<AKeroroWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (CurWeapon)
	{
		// 오프셋 적용 ( 무기마다 자연스러운 포지션 - 추후 데이터테이블 생성 후 꺼내올 예정)
		CurWeapon->SetActorRelativeLocation(CurWeapon->AttachLocationOffset);
		CurWeapon->SetActorRelativeRotation(CurWeapon->AttachRotationOffset);
		CurWeapon->SetActorRelativeScale3D(CurWeapon->AttachScale);
		CurWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
	}

	// HP바 위젯
	auto HpBarWidget = Cast<UKeroroHPBarWidget>(HPBar->GetUserWidgetObject());
	if (HpBarWidget != nullptr)
	{
		HpBarWidget->BindKRStat(KRStat);
	}
}

// Called every frame
void AKeroroCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AKeroroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


float AKeroroCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	KRStat->SetDamage(FinalDamage);
	return FinalDamage;
}

void AKeroroCharacter::Attack()
{
	//UE_LOG(LogTemp, Warning, TEXT("Attack succed"));

	if (IsAttacking) // 애니메이션(몽타주) 재생중인가
	{
		HandleComboInput();
	}
	else
	{
		StartNewAttack();
	}
}

void AKeroroCharacter::HandleComboInput()
{
	if (!FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo)) return;
	if (CanNextCombo)IsComboInputOn = true;
}

void AKeroroCharacter::StartNewAttack()
{
	if (CurrentCombo != 0) return;
	AttackStartComboState();
	KRAnim->PlayAttackMontage();
	KRAnim->JumptoAttackMontageSection(CurrentCombo);
	IsAttacking = true;
}

void AKeroroCharacter::PlaySwordEffect()
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		NSAttackEffect,
		GetActorLocation() + GetActorForwardVector() * 100.0f, // 캐릭터 앞 방향으로 100 유닛 이동
		GetActorRotation(),
		FVector(2.0f)
	);
}

void AKeroroCharacter::BindAnimInstanceEvents()
{

	KRAnim = Cast<UKeroroAnimInstance>(GetMesh()->GetAnimInstance());
	if (KRAnim == nullptr) return;

	// 몽타주 끝났을 시 공격콤보 초기화
	KRAnim->OnMontageEnded.AddDynamic(this, &AKeroroCharacter::OnAttackMontageEnded);

	// 다음 공격 바인딩
	KRAnim->OnNextAttackCheck.AddLambda([this]()->void {
		CanNextCombo = false;
		if (IsComboInputOn)
		{
			AttackStartComboState();
			KRAnim->JumptoAttackMontageSection(CurrentCombo);
			IsAttacking = true;
		}
		});

	// 공격 이펙트 바인딩
	KRAnim->OnEffectCreateCheck.AddUObject(this, &AKeroroCharacter::PlaySwordEffect);

	// 공격 충돌 체크 바인딩
	KRAnim->OnAttackHitCheck.AddUObject(this, &AKeroroCharacter::AttackCheck);
}

void AKeroroCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!IsAttacking || CurrentCombo == 0) return;
	IsAttacking = false;
	AttackEndComboState();
}

void AKeroroCharacter::AttackStartComboState()
{
	CanNextCombo = true;
	IsComboInputOn = false;
	if (FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1)) {
		CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
	}
}

void AKeroroCharacter::AttackEndComboState()
{
	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}

void AKeroroCharacter::AttackCheck()
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

	DrawDebugCapsule(
		GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime
	);

#endif

	if (bHit)
	{
		if (IsValid(HitResult.GetActor()))
		{
			FDamageEvent DamageEvent;
			HitResult.GetActor()->TakeDamage(KRStat->AttackPower*2, DamageEvent, GetController(), this);
			UE_LOG(LogTemp, Warning, TEXT(" hitted : %s"), *HitResult.GetActor()->GetName());

		}
	}
}

void AKeroroCharacter::StartRun()
{
	if (KRAnim != nullptr) {
		KRAnim->bIsRunning = true;
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
}

void AKeroroCharacter::StopRun()
{
	if (KRAnim != nullptr) {
		KRAnim->bIsRunning = false;
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void AKeroroCharacter::LoadAssetandSetting(EKeroroType type)
{
	USkeletalMesh* NewMesh = nullptr;

	switch (type)
	{
	case EKeroroType::Keroro:
		NewMesh = LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Keroro_Model/keroro/keroro.keroro"));
		break;
	case EKeroroType::Tamama:
		NewMesh = LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Keroro_Model/tamama/tamama.tamama"));
		break;
	case EKeroroType::Giroro:
		NewMesh = LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Keroro_Model/giroro/giroro.giroro"));
		break;
	case EKeroroType::Kururu:
		NewMesh = LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Keroro_Model/kururu/kururu.kururu"));
		break;
	case EKeroroType::Dororo:
		NewMesh = LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Keroro_Model/dororo/dororo.dororo"));
		break;
	}

	if (NewMesh)
	{
		GetMesh()->SetSkeletalMesh(NewMesh);

		// 애님 인스턴스 세팅
		UClass* AnimBPClass = LoadClass<UAnimInstance>(nullptr, TEXT("/Game/Blueprints/KR_AnimInstance.KR_AnimInstance_C"));
		if (AnimBPClass)
		{
			GetMesh()->SetAnimInstanceClass(AnimBPClass);
		}

		// 애님인스턴스 설정 및 델리게이트 바인딩
		BindAnimInstanceEvents();
	}
}