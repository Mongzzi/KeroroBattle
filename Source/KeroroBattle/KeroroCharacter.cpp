// Fill out your copyright notice in the Description page of Project Settings.


#include "KeroroCharacter.h"
#include "KeroroEnemyCharacter.h"
#include "KeroroPlayerController.h"
#include "KeroroAnimInstance.h"
#include "KeroroWeapon.h"
#include "KeroballWeapon.h"
#include "SwordWeapon.h"
#include "RifleWeapon.h"
#include "RifleBullet.h"
#include "KeroroPlayerState.h"
#include "KeroroStatComponent.h"
#include "KeroroHPBarWidget.h"
#include "KeroroAIController.h"
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
	SpringArm->TargetArmLength = 250.0f;
	SpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 50.0f, 60.0f), FRotator(-15.0f, 0.0f, 0.0f));

	// 캡슐컴포넌트 콜리전프로파일 설정 
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("KeroroCharacter"));

	// 캐릭터 스탯 컴포넌트
	KRStat = CreateDefaultSubobject<UKeroroStatComponent>(TEXT("KRSTAT"));

	// 나이아가라 이펙트 추가
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NE(TEXT("/Game/Knife_light/VFX/NE_attack02.NE_attack02"));
	if (NE.Succeeded())
	{
		NSSWordEffect = NE.Object;
	}
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NE2(TEXT("/Game/MuzzleFlash/MuzzleFlash/Niagara/NS_MuzzleFlash.NS_MuzzleFlash"));
	if (NE2.Succeeded())
	{
		NSRifleEffect = NE2.Object;
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
	SpringArm->bDoCollisionTest = false; // 벽에 가까이 갔을 때 카메라 충돌 보정

	// 카메라 설정
	Camera->bUsePawnControlRotation = false;

	// 캐릭터 회전 관련 설정
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true; // 입력 방향 따라 몸 회전
	GetCharacterMovement()->bUseControllerDesiredRotation = true; // 컨트롤러 회전 비활성화

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
	AttackRadius = 100.0f;

	// AI설정
	AIControllerClass = AKeroroAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AKeroroCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// (스켈레탈메시,애님인스턴스 로드 후 설정),(몽타주 델리게이트 바인딩)
	LoadAssetandSetting(CurrentKeroroType);
}

// Called when the game starts or when spawned
void AKeroroCharacter::BeginPlay()
{
	Super::BeginPlay();

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

void AKeroroCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	UnbindCharacterEvents();
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

	// 체력이 0이하가 되면 die함수 호출
	if (KRStat->GetHpRatio() <= 0.0f)
	{
		SetActorEnableCollision(false);
		SetLifeSpan(5.0f);
		Die();
	}
	return FinalDamage;
}

void AKeroroCharacter::Die()
{
	KRAnim->SetDeadAnim();

	AKeroroPlayerController* PC = Cast<AKeroroPlayerController>(GetController());
	if (PC)
	{
		// 플레이어 컨트롤러에서 다음 캐릭으로 포제스하고  캐릭터 맵 목록 업데이트
		PC->Die();
	}
	// 바인딩된 함수들 다 해제
	UnbindCharacterEvents();
	// 콜리전 끄기
	SetActorEnableCollision(false);
	// 일정 시간 후 소멸
	SetLifeSpan(5.0f);
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

	if (WeaponType == EWeaponType::RIFLE)
	{
		FRotator ControlRotation = GetControlRotation();
		ControlRotation.Pitch = 0.0f;
		ControlRotation.Roll = 0.0f;
		SetActorRotation(ControlRotation);

	}

	AttackStartComboState();
	KRAnim->PlayAttackMontage();
	KRAnim->JumptoAttackMontageSection(CurrentCombo);
	IsAttacking = true;
}

void AKeroroCharacter::SetWeapon()
{
	if (Weapon)
	{
		Weapon->Destroy();
	}

	switch (WeaponType)
	{
	case EWeaponType::FIST:
		Weapon = nullptr;
		break;
	case EWeaponType::KEROBALL:
		Weapon = GetWorld()->SpawnActor<AKeroballWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
		break;
	case EWeaponType::RIFLE:
		Weapon = GetWorld()->SpawnActor<ARifleWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
		break;
	case EWeaponType::SWORD:
		Weapon = GetWorld()->SpawnActor<ASwordWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
		break;
	case EWeaponType::MAX:
		break;
	}
	if (Weapon)
	{
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, Weapon->GetSocketName());
	}
}

void AKeroroCharacter::PlaySwordEffect()
{
	if (WeaponType == EWeaponType::SWORD)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NSSWordEffect,
			GetActorLocation() + GetActorForwardVector() * 100.0f, // 캐릭터 앞 방향으로 100 유닛 이동
			GetActorRotation(),
			FVector(2.0f)
		);
	}
	else if (WeaponType == EWeaponType::RIFLE)
	{
		if (Weapon && Cast<ARifleWeapon>(Weapon)) {
			UNiagaraComponent* RifleEffect = UNiagaraFunctionLibrary::SpawnSystemAttached(
				NSRifleEffect,
				Weapon->SKMeshComponent,
				"Muzzle",
				FVector::ZeroVector,
				FRotator(0.0f, 0.0f, 90.0f),
				EAttachLocation::SnapToTarget,
				true
			);
			if (RifleEffect)
			{
				RifleEffect->SetRelativeScale3D(FVector(300.0f));
				RifleEffect->Activate();

				// 람다에서 안전하게 이펙트 관리
				TWeakObjectPtr<UNiagaraComponent> WeakRifleEffect = RifleEffect;
				FTimerHandle EffectTimerHandle;
				GetWorld()->GetTimerManager().SetTimer(EffectTimerHandle, [WeakRifleEffect]()
					{
						if (WeakRifleEffect.IsValid())
						{
							WeakRifleEffect->Deactivate();
						}
					}, 1.0f, false);
			}
		}
	}
}

void AKeroroCharacter::BindCharacterEvents()
{
	UnbindCharacterEvents();
	if (KRAnim)
	{
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

		// 무기 다시생성
		KRAnim->OnNextAttackCheck.AddUObject(this, &AKeroroCharacter::SpawnToHand);

	}

	if (KRStat)
	{
		// 스탯컴포넌트 체력0 델리게이트 바인딩
		KRStat->OnHpIsZero.AddUObject(this, &AKeroroCharacter::Die);
	}
}

void AKeroroCharacter::UnbindCharacterEvents()
{
	if (KRAnim)
	{
		// 몽타주 끝났을 시 공격콤보 초기화 해제
		KRAnim->OnMontageEnded.RemoveDynamic(this, &AKeroroCharacter::OnAttackMontageEnded);
		// 다음 공격 바인딩 해제
		KRAnim->OnNextAttackCheck.RemoveAll(this);
		// 공격 이펙트 바인딩 해제
		KRAnim->OnEffectCreateCheck.RemoveAll(this);
		// 공격 충돌 체크 바인딩 해제
		KRAnim->OnAttackHitCheck.RemoveAll(this);
	}

	if (KRStat)
	{
		KRStat->OnHpIsChanged.RemoveAll(this);
	}
}


void AKeroroCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!IsAttacking || CurrentCombo == 0) return;
	IsAttacking = false;
	AttackEndComboState();
	OnAttaackEnd.Broadcast();
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

	switch (WeaponType)
	{
	case EWeaponType::FIST:
		break;
	case EWeaponType::KEROBALL:
		AttackCheck_Keroball();
		break;
	case EWeaponType::RIFLE:
		AttackCheck_Rifle();
		break;
	case EWeaponType::SWORD:
		AttackCheck_Sword();
		break;
	}
}

void AKeroroCharacter::AttackCheck_Sword()
{
	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this); // 자기 자신은 무시

	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		GetActorLocation() + GetActorForwardVector() * AttackRadius / 2,	// 시작위치 - 구형태이므로 반지름(attack radios)이 커지면 캐릭터 뒤쪽도 공격판정들어감
		GetActorLocation() + GetActorForwardVector() * AttackRange,	// 끝위치
		FQuat::Identity, // 회전 없음
		ECC_GameTraceChannel3, // Attack채널 ( DefaultEngine 파일에 내가만든 채널 몇번쨰인지 나와있음 )
		FCollisionShape::MakeSphere(AttackRadius),
		Params
	);

	if (bHit)
	{
		for (const FHitResult& Hit : HitResults)
		{
			AActor* HitActor = Hit.GetActor();
			if (IsValid(HitActor) && Cast<AKeroroEnemyCharacter>(Hit.GetActor()))
			{
				FDamageEvent DamageEvent;
				HitActor->TakeDamage(KRStat->AttackPower * 5, DamageEvent, GetController(), this);
				//UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *HitActor->GetName());
			}
		}
	}
}

void AKeroroCharacter::AttackCheck_Rifle()
{
	FRotator MuzzleRotation = GetControlRotation();
	MuzzleRotation.Pitch = 0.0f;
	FVector MuzzleOffset = FVector(100.f, 0.0f, 25.0f);
	FVector MuzzleLocation = GetActorLocation() + MuzzleRotation.RotateVector(MuzzleOffset);
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	//SpawnParams.Owner = this;
	SpawnParams.Instigator = this;
	GetWorld()->SpawnActor<ARifleBullet>(ARifleBullet::StaticClass(), MuzzleLocation, MuzzleRotation, SpawnParams);
	Weapon->PlaySound();
}

void AKeroroCharacter::AttackCheck_Keroball()
{
	if (Weapon)
	{
		UE_LOG(LogTemp, Error, TEXT("attack check Keroball "));
		FRotator ControlRot = GetControlRotation();
		ControlRot.Pitch = 1.0f;

		// 던지는 방향 (컨트롤러 앞방향)
		FVector ThrowDir = ControlRot.Vector();
		Weapon->Throw(ThrowDir, 1000.0f);
	}
}

void AKeroroCharacter::SpawnToHand()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = this;
	Weapon = GetWorld()->SpawnActor<AKeroballWeapon>(FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if (Weapon)
	{
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, Weapon->GetSocketName());
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

	CurrentKeroroType = type;

	switch (type)
	{
	case EKeroroType::Keroro:
		NewMesh = LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Keroro_Model/keroro/keroro.keroro"));
		WeaponType = EWeaponType::KEROBALL;
		break;
	case EKeroroType::Tamama:
		NewMesh = LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Keroro_Model/tamama/tamama.tamama"));
		WeaponType = EWeaponType::FIST;
		break;
	case EKeroroType::Giroro:
		NewMesh = LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Keroro_Model/giroro/giroro.giroro"));
		WeaponType = EWeaponType::RIFLE;
		break;
	case EKeroroType::Kururu:
		NewMesh = LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Keroro_Model/kururu/kururu.kururu"));
		WeaponType = EWeaponType::FIST;
		break;
	case EKeroroType::Dororo:
		NewMesh = LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Keroro_Model/dororo/dororo.dororo"));
		WeaponType = EWeaponType::SWORD;
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

		KRAnim = Cast<UKeroroAnimInstance>(GetMesh()->GetAnimInstance());
		KRAnim->SetWeaponType(WeaponType);
		// 애님인스턴스 설정 및 델리게이트 바인딩
		BindCharacterEvents();
		SetWeapon();
	}
}