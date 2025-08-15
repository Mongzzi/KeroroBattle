// Fill out your copyright notice in the Description page of Project Settings.


#include "KeroroCharacter.h"
#include "KeroroEnemyCharacter.h"
#include "KeroroPlayerController.h"
#include "KeroroAnimInstance.h"
#include "KeroroWeapon.h"
#include "KeroroGameInstance.h"
#include "KeroballWeapon.h"
#include "SwordWeapon.h"
#include "RotateWeapon.h"
#include "FistWeapon.h"
#include "ImpactWeapon.h"
#include "RifleWeapon.h"
#include "MineWeapon.h"
#include "NoteBookWeapon.h"
#include "KR_MovingObject.h"
#include "RifleBullet.h"
#include "KeroroPlayerState.h"
#include "CriticalDamageType.h"
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
#include "Engine/StreamableManager.h"
#include "Kismet/GameplayStatics.h"
#include "DamageTextWidget.h"


// Sets default values
AKeroroCharacter::AKeroroCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 메시 위치 조정
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));

	// 카메라 스프링암
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	SpringArm->SetupAttachment(GetCapsuleComponent());

	SpringArm->TargetArmLength = 250.0f;
	SpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 50.0f, 60.0f), FRotator(-15.0f, 0.0f, 0.0f));

	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bInheritRoll = false;
	SpringArm->bDoCollisionTest = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	Camera->bUsePawnControlRotation = false;
	Camera->SetupAttachment(SpringArm);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 캡슐컴포넌트 콜리전프로파일 설정 
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("KeroroCharacter"));

	// 캐릭터 스탯 컴포넌트
	KRStat = CreateDefaultSubobject<UKeroroStatComponent>(TEXT("KRSTAT"));

	// 나이아가라 이펙트 추가

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NE2(TEXT("/Game/MuzzleFlash/MuzzleFlash/Niagara/NS_MuzzleFlash.NS_MuzzleFlash"));
	if (NE2.Succeeded())
	{
		NSRifleEffect = NE2.Object;
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NE3(TEXT("/Game/KTP_Effect/Particles/Fly/Expolison_06_09.Expolison_06_09"));
	if (NE3.Succeeded())
	{
		NSShieldEffect = NE3.Object;
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NE4(TEXT("/Game/MixedVFX/Particles/Slashes/SeparateParts/Hits/NS_LightningSlash_Hit.NS_LightningSlash_Hit"));
	if (NE4.Succeeded())
	{
		NSGuardEffect = NE4.Object;
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NE5(TEXT("/Game/KTP_Effect/Particles/Fly/Others/energy_06_01.energy_06_01"));
	if (NE5.Succeeded())
	{
		NSHPMPEffect = NE5.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS1(TEXT("/Game/FXVarietyPack/Particles/P_ky_hit1.P_ky_hit1"));
	if (PS1.Succeeded())
	{
		PSParryEffect = PS1.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> SOUND1(TEXT("/Game/Keroro_Sound/etc/ShieldSound.ShieldSound"));
	if (SOUND1.Succeeded())
	{
		ShieldSound = SOUND1.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> SOUND2(TEXT("/Game/Keroro_Sound/etc/GuardSound.GuardSound"));
	if (SOUND2.Succeeded())
	{
		GuardSound = SOUND2.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> SOUND3(TEXT("/Game/Keroro_Sound/etc/ParrySound.ParrySound"));
	if (SOUND3.Succeeded())
	{
		ParrySound = SOUND3.Object;
	}

	static ConstructorHelpers::FClassFinder<UDamageTextWidget> DAMAGETEXT(TEXT("/Game/Blueprints/KR_DamageWidget.KR_DamageWidget_C"));
	if (DAMAGETEXT.Succeeded())
	{
		DamageTextWidgetClass = DAMAGETEXT.Class;
	}

	// HP바 추가
	HPBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));
	HPBar->SetupAttachment(GetMesh());
	HPBar->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	HPBar->SetWidgetSpace(EWidgetSpace::Screen);
	HPBar->SetDrawSize(FVector2D(300.0f, 50.0f));
	static ConstructorHelpers::FClassFinder<UUserWidget>HUD(TEXT("/Game/Blueprints/KR_HPBar.KR_HPBar_C"));
	if (HUD.Succeeded()) HPBar->SetWidgetClass(HUD.Class);


	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;

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
	//UE_LOG(LogTemp, Warning, TEXT("post called"));

	// (스켈레탈메시,애님인스턴스 로드 후 설정),(몽타주 델리게이트 바인딩)
	LoadAssetandSetting(CurrentKeroroType);
}

void AKeroroCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (KRPlayerState == nullptr)
	{
		KRPlayerState = Cast<AKeroroPlayerState>(NewController->PlayerState);
		if (KRPlayerState)
		{
			UE_LOG(LogTemp, Warning, TEXT("KRPlayerState is valid in PossessedBy"));

			KRStat->SetLevel(KRPlayerState->CurrentLevel);
			KRStat->StartAutoHeal();
			KRStat->StartAutoMpHeal();
		}

	}
}

// Called when the game starts or when spawned
void AKeroroCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HPBar)
	{
		auto HpBarWidget = Cast<UKeroroHPBarWidget>(HPBar->GetUserWidgetObject());
		if (HpBarWidget != nullptr)
		{
			HpBarWidget->BindKRStat(KRStat);
			HpBarWidget->SetHPBarTextVisible();
		}
	}

}

// Called every frame
void AKeroroCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//if (KRAnim)
	//{
	//	UE_LOG(LogTemp,Error,TEXT("bishit = %d"),KRAnim->bIsHit);
	//}
}

void AKeroroCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnbindCharacterEvents();
	Super::EndPlay(EndPlayReason);
}

// Called to bind functionality to input
void AKeroroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


float AKeroroCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (KRStat == nullptr || KRAnim == nullptr) return 0.0f;

	if (ParryCheck()) return 0.0f;
	if (GuardCheck()) return 0.0f;

	// 데미지 처리
	FDamageResult DamageResult = KRStat->SetFinalDamage(Damage);

	// 회피
	if (DamageResult.ResultType == EDamageResultType::Evaded)
	{
		ShowDamageTextMiss();
		return 0.0f;
	}
	// 무적시간
	else if (DamageResult.ResultType == EDamageResultType::Invincible)
	{
		ShowDamageTextinvincible();
		return 0.0f;
	}

	float FinalDamage = DamageResult.FinalDamage;
	float FinalDamagePercent = FinalDamage / KRStat->MaxHp;

	// 크리티컬 데미지 입을 시
	if (DamageEvent.DamageTypeClass && DamageEvent.DamageTypeClass->GetDefaultObject()->IsA(UCriticalDamageType::StaticClass()))
	{
		KRAnim->SetbIsHit(CurrentKeroroType);
		ChangeFaceTexture(EFaceType::Anger);
		if (Cast<AKeroroPlayerController>(GetController()))
		{
			PlayHittedSound();
		}
	}

	ShowDamageText(DamageEvent, FinalDamage);

	// 플레이어 사망
	if (KRStat->GetHpRatio() <= 0.0f)
	{
		Die();
	}

	return FinalDamage;
}

void AKeroroCharacter::Die()
{
	IsDie = true;
	KRAnim->SetDeadAnim();

	if (AKeroroPlayerController* PC = Cast<AKeroroPlayerController>(GetController()))
	{
		PC->Die();
	}
	else if (AKeroroAIController* AC = Cast<AKeroroAIController>(GetController()))
	{
		AKeroroPlayerController* PC2 = Cast<AKeroroPlayerController>(GetWorld()->GetFirstPlayerController());
		if (PC2) {
			PC2->DieAIKero(CurrentKeroroType);
		}
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
	if (KRAnim)
	{
		if (KRAnim->bIsHit) return;
	}

	LookAttackDir();

	if (IsAttacking) // 애니메이션(몽타주) 재생중인가
	{
		HandleComboInput();
	}
	else
	{
		StartNewAttack();
	}

	//int RandomIndex = FMath::RandRange(0, 5);
	//ChangeFaceTexture(static_cast<EFaceType>(RandomIndex));
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

	if (Weapon && WeaponType == EWeaponType::KEROBALL)
	{
		if (Cast<AKeroballWeapon>(Weapon)->bIsThrowing)
		{
			SpawnToHand();
		}
	}
}

void AKeroroCharacter::PlayVoiceSound()
{
	if (Cast<AKeroroPlayerController>(GetController()))
	{
		if (WeaponType == EWeaponType::RIFLE)
		{
			int RandomIndex = FMath::RandRange(0, 10);	//0~2 유효 5~10 무효 ,3은 그냥뻇음 4는 궁극기, 무효시 아무소리안나게 너무시끄러움..
			if (VoiceSounds.IsValidIndex(RandomIndex) && RandomIndex != 4)
			{
				UGameplayStatics::PlaySoundAtLocation(this, VoiceSounds[RandomIndex], GetActorLocation());
				return;
			}
		}
		else if (VoiceSounds.IsValidIndex(CurrentCombo - 1))
		{
			UGameplayStatics::PlaySoundAtLocation(this, VoiceSounds[CurrentCombo - 1], GetActorLocation());
		}
	}
}

void AKeroroCharacter::PlayUltiSkillSound()
{
	// 상수로 넣어주고있어서 살짝 위험함 추후 수정 필요
	if (VoiceSounds.IsValidIndex(4))
	{
		UGameplayStatics::PlaySoundAtLocation(this, VoiceSounds[4], GetActorLocation());
	}
}

void AKeroroCharacter::PlayHittedSound()
{
	if (VoiceSounds.IsValidIndex(5))
	{
		UGameplayStatics::PlaySoundAtLocation(this, VoiceSounds[5], GetActorLocation(), 0.6f);
	}
}

void AKeroroCharacter::PlayHitSound(int32 Combo)
{
	if (Weapon)
	{
		Weapon->PlayHitSound(Combo);
	}
}

void AKeroroCharacter::PlayHitEffect(FVector HitLocation, FRotator HitRotator, FVector Scale)
{
	if (Weapon)
	{
		Weapon->PlayHitEffect(HitLocation, HitRotator, Scale);
	}
}

void AKeroroCharacter::PlayHealEffect()
{
	if (NSHPMPEffect)
	{
		if (NCHPMPEffect)
		{
			NCHPMPEffect->Deactivate();
			NCHPMPEffect->DestroyComponent();
			NCHPMPEffect = nullptr;
		}

		NCHPMPEffect = UNiagaraFunctionLibrary::SpawnSystemAttached(
			NSHPMPEffect,
			GetMesh(),
			NAME_None,
			FVector(0.0f, 0.0f, 50.0f),
			FRotator(0.0f, 0.0f, 90.0f),
			EAttachLocation::KeepRelativeOffset,
			true);
		NCHPMPEffect->SetRelativeScale3D(FVector(0.1f));
	}
}

void AKeroroCharacter::PlayWeaponSound()
{
	if (Weapon)
	{
		Weapon->PlaySound(CurrentCombo);
	}
}

void AKeroroCharacter::HiddenHPBarOnHead()
{
	if (HPBar)
	{
		HPBar->SetVisibility(false);
	}
}

void AKeroroCharacter::KeroBallUlti()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnParams.Instigator = this;

	for (int i = 0; i < 100; ++i)
	{
		float Randx = FMath::FRandRange(-2000, 2000.0f);
		float Randy = FMath::FRandRange(-2000, 2000.0f);
		float Randz = FMath::FRandRange(1000.0f, 2000.0f);

		auto keroball = GetWorld()->SpawnActor<AKeroballWeapon>(GetActorLocation() + FVector(Randx, Randy, Randz), FRotator::ZeroRotator, SpawnParams);
		if (keroball) {
			keroball->FallDown();
		}
	}
}

void AKeroroCharacter::SwordUlti()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnParams.Instigator = this;
	UltiWeapon = GetWorld()->SpawnActor<ARotateWeapon>(FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	ARotateWeapon* RotateWeapon = Cast<ARotateWeapon>(UltiWeapon);
	RotateWeapon->PlayEffect(this);
}

void AKeroroCharacter::FistUlti()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnParams.Instigator = this;
	UltiWeapon = GetWorld()->SpawnActor<AImpactWeapon>(FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	UltiWeapon->PlayEffect(this);
}

void AKeroroCharacter::NoteBookUlti()
{
	FVector Start = GetActorLocation() + FVector(0.0f, 0.0f, 100.0f);

	FVector Forward = GetActorForwardVector();
	FVector Right = GetActorRightVector();
	FVector Up = GetActorUpVector();

	FVector Mid = Start
		+ Forward * FMath::FRandRange(4000.0f, 5000.0f)
		+ Right * FMath::FRandRange(-5000.0f, 5000.0f)
		+ Up * FMath::FRandRange(500.0f, 1500.0f);

	FVector End = Mid
		+ Forward * FMath::FRandRange(4000.0f, 5000.0f)
		+ Right * FMath::FRandRange(-5000.0f, 5000.0f)
		+ Up * FMath::FRandRange(500.0f, 1500.0f);

	AKR_MovingObject* Obj = GetWorld()->SpawnActor<AKR_MovingObject>(AKR_MovingObject::StaticClass(), Start, FRotator::ZeroRotator);

	if (Obj)
	{
		Obj->SetBezierPoints(Start, Mid, End);
		Obj->StartMoving(2000.0f);
	}
}

void AKeroroCharacter::RifleUlti()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnParams.Instigator = this;
	UltiWeapon = GetWorld()->SpawnActor<AMineWeapon>(GetActorLocation(), GetActorRotation(), SpawnParams);
	UltiWeapon->BindOwnerKero(this);
}

void AKeroroCharacter::ChangeCameraDefault()
{
	if (!SpringArm || !Camera) return;

	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bInheritRoll = false;
	SpringArm->bDoCollisionTest = false;
	SpringArm->TargetArmLength = 250.0f;
	SpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 50.0f, 60.0f), FRotator(-15.0f, 0.0f, 0.0f));

	Camera->bUsePawnControlRotation = false;
	Camera->SetupAttachment(SpringArm);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	auto pc = GetController();
	if (pc)
	{
		FRotator ControlRot = pc->GetControlRotation();
		ControlRot.Pitch = 0.0f;
		pc->SetControlRotation(ControlRot);
	}

}

void AKeroroCharacter::ChangeCameraUltimate()
{
	switch (CurrentKeroroType)
	{
	case EKeroroType::Keroro:
		//Camera->bUsePawnControlRotation = false;
		//SpringArm->bInheritYaw = false;
		//SpringArm->bInheritPitch = false;
		//SpringArm->TargetArmLength = 400.0f;
		//SpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(-30.0f, 180.0f, 0.0f));
		break;
	case EKeroroType::Tamama:
		SpringArm->TargetArmLength = 150.0f;
		SpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 40.0f), FRotator(-30.0f, 0.0f, 0.0f));
		break;
	case EKeroroType::Giroro:
		break;
	case EKeroroType::Kururu:
		break;
	case EKeroroType::Dororo:
		SpringArm->bInheritPitch = false;
		SpringArm->TargetArmLength = 1000.0f;
		SpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(-45.0f, 0.0f, 0.0f));
		break;
	}
}

void AKeroroCharacter::LookAttackDir()
{
	// 공격 시작시 컨트롤러 방향으로 캐릭터 회전
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0.0f;
	ControlRotation.Roll = 0.0f;
	SetActorRotation(ControlRotation);
	GetCharacterMovement()->bOrientRotationToMovement = false; // 입력 방향 따라 몸 회전 안되게
}

void AKeroroCharacter::ChangeCameraNoteBookAttack()
{
	SpringArm->bInheritPitch = false;
	SpringArm->TargetArmLength = 1500.0f;
	SpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(-25.0f, 0.0f, 0.0f));
	//SpringArm->bInheritYaw = false;
}

float AKeroroCharacter::GetRemainingGuardCooldown()
{
	if (!GetWorld()) return 0.0f;
	return GetWorld()->GetTimerManager().GetTimerRemaining(GuardCooldownTimer);
}

void AKeroroCharacter::SetUltiCooldownReset()
{
	if (!GetWorld())return;
	GetWorld()->GetTimerManager().ClearTimer(UltimateSkillCooldownTimer);

	//auto a = GetWorld()->GetTimerManager().GetTimerRemaining(UltimateSkillCooldownTimer);
	//UE_LOG(LogTemp, Error, TEXT("Return value %f"), a);
}

void AKeroroCharacter::SetGuardCooldownReset()
{
	if (!GetWorld())return;
	GetWorld()->GetTimerManager().ClearTimer(GuardCooldownTimer);
	CanGuarding = true;
}

float AKeroroCharacter::GetRemainingUltimateSkillCooldown()
{
	if (!GetWorld()) return 0.0f;
	return GetWorld()->GetTimerManager().GetTimerRemaining(UltimateSkillCooldownTimer);
}

void AKeroroCharacter::SetWeapon()
{
	if (Weapon)
	{
		Weapon->Destroy();
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnParams.Instigator = this;
	SpawnParams.Owner = this;

	switch (WeaponType)
	{
	case EWeaponType::FIST:
		Weapon = GetWorld()->SpawnActor<AFistWeapon>(FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		Cast<AFistWeapon>(Weapon)->InitEffect(this);
		break;
	case EWeaponType::KEROBALL:
		Weapon = GetWorld()->SpawnActor<AKeroballWeapon>(FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		break;
	case EWeaponType::RIFLE:
		Weapon = GetWorld()->SpawnActor<ARifleWeapon>(FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		break;
	case EWeaponType::SWORD:
		Weapon = GetWorld()->SpawnActor<ASwordWeapon>(FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		break;
	case EWeaponType::NOTEBOOK:
		Weapon = GetWorld()->SpawnActor<ANoteBookWeapon>(FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		Cast<ANoteBookWeapon>(Weapon)->BindOwnerKero(this);
		break;
	}
	if (Weapon)
	{
		TArray<FName> SocketNames = Weapon->GetSocketNames();
		if (SocketNames.IsValidIndex(0))
		{
			Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, SocketNames[0]);
		}
	}
}

void AKeroroCharacter::PlayEffect()
{
	if (WeaponType == EWeaponType::SWORD)
	{
		Cast<ASwordWeapon>(Weapon)->PlayEffect(this);
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
	// 기존 이벤트 해제 (중복 바인딩 방지)
	UnbindCharacterEvents();

	// 애님 인스턴스 유효성 체크
	if (IsValid(KRAnim))
	{
		// 몽타주 끝났을 시 공격 콤보 초기화
		KRAnim->OnMontageEnded.AddDynamic(this, &AKeroroCharacter::OnAttackMontageEnded);

		// 다음 공격 바인딩
		KRAnim->OnNextAttackCheck.AddLambda([this]()
			{
				if (!IsValid(this) || !IsValid(KRAnim)) return; // 람다 내부 재검사

				CanNextCombo = false;
				if (IsComboInputOn)
				{
					AttackStartComboState();
					KRAnim->JumptoAttackMontageSection(CurrentCombo);
					IsAttacking = true;
				}
			});

		// 공격 이펙트 바인딩
		KRAnim->OnEffectCreateCheck.AddUObject(this, &AKeroroCharacter::PlayEffect);

		// 공격 충돌 체크 바인딩
		KRAnim->OnAttackHitCheck.AddUObject(this, &AKeroroCharacter::AttackCheck);

		// 캐릭터 보이스 체크 바인딩
		KRAnim->OnVoiceCheck.AddUObject(this, &AKeroroCharacter::PlayVoiceSound);

		// 무기 공격 사운드 바인딩
		KRAnim->OnWeaponSoundCheck.AddUObject(this, &AKeroroCharacter::PlayWeaponSound);

		// 무기 다시 생성 (케로볼일 경우)
		if (WeaponType == EWeaponType::KEROBALL)
		{
			KRAnim->OnNextAttackCheck.AddUObject(this, &AKeroroCharacter::SpawnToHand);
		}
	}

	// 스탯 컴포넌트 유효성 체크
	if (IsValid(KRStat))
	{
		KRStat->OnHpIsZero.AddUObject(this, &AKeroroCharacter::Die);
	}
}

void AKeroroCharacter::UnbindCharacterEvents()
{
	if (IsValid(KRAnim))
	{
		KRAnim->OnMontageEnded.RemoveDynamic(this, &AKeroroCharacter::OnAttackMontageEnded);
		KRAnim->OnNextAttackCheck.RemoveAll(this);
		KRAnim->OnEffectCreateCheck.RemoveAll(this);
		KRAnim->OnAttackHitCheck.RemoveAll(this);
		KRAnim->OnVoiceCheck.RemoveAll(this);
		KRAnim->OnWeaponSoundCheck.RemoveAll(this);
	}

	if (IsValid(KRStat))
	{
		KRStat->OnHpIsZero.RemoveAll(this);
	}

	GetWorld()->GetTimerManager().ClearTimer(ParryTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(GuardTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(GuardCooldownTimer);
	GetWorld()->GetTimerManager().ClearTimer(UltimateSkillCooldownTimer);
}

void AKeroroCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!IsAttacking || CurrentCombo == 0) return;
	IsAttacking = false;
	GetCharacterMovement()->bOrientRotationToMovement = true; // 입력 방향 따라 몸 회전
	AttackEndComboState();
	//OnAttaackEnd.Broadcast();
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
		AttackCheck_Fist();
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
	case EWeaponType::NOTEBOOK:
		AttackCheck_NoteBook();
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
				float FinalDamage = KRStat->AttackPower;
				bool bIsCritical = (FMath::FRand() < KRStat->CritChanceRate);

				FDamageEvent DamageEvent;
				if (bIsCritical)
				{
					FinalDamage *= KRStat->CritDamageRate;
					DamageEvent.DamageTypeClass = UCriticalDamageType::StaticClass();
				}

				HitActor->TakeDamage(FinalDamage * 2, DamageEvent, GetController(), this);

				PlayHitEffect(Hit.ImpactPoint, Hit.ImpactNormal.Rotation(), FVector(0.5f));
			}
		}
		PlayHitSound();
	}
}

void AKeroroCharacter::AttackCheck_Rifle()
{
	if (!KRStat)
	{
		UE_LOG(LogTemp, Error, TEXT("KRStat in null in AttackCheck_Rifle"));
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnParams.Instigator = this;

	int32  BulletNum = KRStat->ProjectileCount;
	float BulletScale = KRStat->ProjectileScale;

	float MaxAngle = 45.0f;
	float BaseYaw = GetControlRotation().Yaw;
	float AddYaw = (BulletNum > 1) ? (MaxAngle) / (BulletNum - 1) : 0.0f;

	TArray<int> BulletSequence;	// 불릿 스폰 순서 담는 컨테이너 좌우 좌우 반복하며 생성
	BulletSequence.Add(0); // 중앙부터 시작
	for (int i = 1; i <= BulletNum / 2; i++)
	{
		if (BulletSequence.Num() < BulletNum) BulletSequence.Add(i);
		if (BulletSequence.Num() < BulletNum) BulletSequence.Add(-i);
	}

	for (int i = 0; i < BulletSequence.Num(); i++)
	{
		int Adjust = BulletSequence[i];
		float YawOffset = Adjust * AddYaw;

		FRotator MuzzleRotation = GetControlRotation();
		MuzzleRotation.Pitch += 10.0f;
		MuzzleRotation.Yaw = BaseYaw + YawOffset;

		FVector MuzzleOffset = FVector(100.f, 0.0f, 25.0f);
		FVector MuzzleLocation = GetActorLocation() + MuzzleRotation.RotateVector(MuzzleOffset);


		ARifleBullet* Bullet = GetWorld()->SpawnActor<ARifleBullet>(ARifleBullet::StaticClass(), MuzzleLocation, MuzzleRotation, SpawnParams);
		Bullet->SetActorScale3D(FVector(BulletScale));
	}


	// 총기 발사 사운드
	Weapon->PlaySound(CurrentCombo);
}

void AKeroroCharacter::AttackCheck_Keroball()
{
	if (Weapon)
	{
		FRotator ControlRot = GetControlRotation();
		ControlRot.Pitch = 0.0f;

		// 던지는 방향 (컨트롤러 앞방향)
		FVector ThrowDir = ControlRot.Vector();
		Weapon->Throw(ThrowDir, 500.0f);
	}
}

void AKeroroCharacter::AttackCheck_Fist()
{

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation() + GetActorForwardVector() * AttackRadius / 2,	// 시작위치 - 구형태이므로 반지름(attack radios)이 커지면 캐릭터 뒤쪽도 공격판정들어감
		GetActorLocation() + GetActorForwardVector() * AttackRange,	// 
		FQuat::Identity, // 회전 없음
		ECC_GameTraceChannel3,
		FCollisionShape::MakeSphere(AttackRadius),
		Params
	);

	if (bHit)
	{
		AActor* HitActor = HitResult.GetActor();
		if (IsValid(HitActor) && HitActor->IsA(AKeroroEnemyCharacter::StaticClass()))
		{

			float FinalDamage = KRStat->AttackPower;
			bool bIsCritical = (FMath::FRand() < KRStat->CritChanceRate);

			FDamageEvent DamageEvent;
			if (bIsCritical)
			{
				FinalDamage *= KRStat->CritDamageRate;
				DamageEvent.DamageTypeClass = UCriticalDamageType::StaticClass();
			}

			HitActor->TakeDamage(FinalDamage * 3, DamageEvent, GetController(), this);

			PlayHitEffect(HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation());
			PlayHitSound(CurrentCombo);

		}
	}
}

void AKeroroCharacter::AttackCheck_NoteBook()
{
	if (WeaponType == EWeaponType::NOTEBOOK)
	{
		Cast<ANoteBookWeapon>(Weapon)->ActivateFinalEffect();
	}
}

bool AKeroroCharacter::ParryCheck()
{
	// 패링 체크 
	if (IsParrying)
	{
		ParryAttack();

		DestroyShieldEffect();

		PCParryEffect = UGameplayStatics::SpawnEmitterAttached(PSParryEffect, GetMesh(), NAME_None,
			FVector(0.0f, 70.0f, 100.0f),
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			true
		);
		PCParryEffect->SetRelativeScale3D(FVector(1.7f));

		IsParrying = false;
		KRAnim->bIsGuarding = false;

		UGameplayStatics::SpawnSoundAtLocation(this, ParrySound, GetActorLocation(), FRotator::ZeroRotator, 1.0f);

		AKeroroPlayerController* PC = Cast<AKeroroPlayerController>(GetController());
		if (PC)
		{
			// 강조선 위젯 애니메이션 출력
			PC->PlayParryWidgetEffect();

			// 카메라 쉐이크 출력
			PC->PlayParryCameraShake();

			// 패리 테스트 표기
			UDamageTextWidget* DamageWidget = CreateWidget<UDamageTextWidget>(PC, DamageTextWidgetClass);
			if (DamageWidget)
			{
				DamageWidget->AddToViewport();
				DamageWidget->SetTargetLocation(GetActorLocation());
				DamageWidget->SetTextParry(); // MISS 텍스트 표시
			}

		}
		return true;
	}
	else {
		return false;
	}
}

bool AKeroroCharacter::GuardCheck()
{
	// 가드 체크
	if (IsGuarding)
	{
		DestroyShieldEffect();

		NCGuardEffect = UNiagaraFunctionLibrary::SpawnSystemAttached(
			NSGuardEffect,
			GetMesh(),
			NAME_None,
			FVector(0.0f, 20.0f, 100.0f),
			FRotator(0.0f, 90.0f, 0.0f),
			EAttachLocation::KeepRelativeOffset,
			true
		);
		IsGuarding = false;
		KRAnim->bIsGuarding = false;

		UGameplayStatics::SpawnSoundAtLocation(this, GuardSound, GetActorLocation(), FRotator::ZeroRotator, 1.0f);

		AKeroroPlayerController* PC = Cast<AKeroroPlayerController>(GetController());
		if (PC)
		{
			UDamageTextWidget* DamageWidget = CreateWidget<UDamageTextWidget>(PC, DamageTextWidgetClass);
			if (DamageWidget)
			{
				DamageWidget->AddToViewport();
				DamageWidget->SetTargetLocation(GetActorLocation());
				DamageWidget->SetTextGuard();
			}
		}
		return true;
	}
	else {
		return false;
	}
}

void AKeroroCharacter::ShowDamageText(FDamageEvent const& DamageEvent, float Damage)
{
	AKeroroPlayerController* PC = Cast<AKeroroPlayerController>(GetController());
	if (!PC) return;
	UDamageTextWidget* DamageWidget = CreateWidget<UDamageTextWidget>(PC, DamageTextWidgetClass);
	DamageWidget->AddToViewport();
	DamageWidget->SetTargetLocation(GetActorLocation());
	if (DamageEvent.DamageTypeClass && DamageEvent.DamageTypeClass->GetDefaultObject()->IsA(UCriticalDamageType::StaticClass()))
	{
		DamageWidget->SetTextFromCritDamage(Damage);
	}
	else {
		DamageWidget->SetTextFromDamage(Damage);
	}
}

void AKeroroCharacter::ShowDamageTextinvincible()
{
	AKeroroPlayerController* PC = Cast<AKeroroPlayerController>(GetController());
	if (!PC) return;

	UDamageTextWidget* DamageWidget = CreateWidget<UDamageTextWidget>(PC, DamageTextWidgetClass);
	DamageWidget->AddToViewport();
	DamageWidget->SetTargetLocation(GetActorLocation());
	DamageWidget->SetTextinvincible();
}

void AKeroroCharacter::ShowDamageTextMiss()
{
	AKeroroPlayerController* PC = Cast<AKeroroPlayerController>(GetController());
	if (!PC) return;

	UDamageTextWidget* DamageWidget = CreateWidget<UDamageTextWidget>(PC, DamageTextWidgetClass);
	DamageWidget->AddToViewport();
	DamageWidget->SetTargetLocation(GetActorLocation());
	DamageWidget->SetTextMiss();
}

void AKeroroCharacter::ShowHealText(float Hp, float Mp)
{
	AKeroroPlayerController* PC = Cast<AKeroroPlayerController>(GetController());
	if (!PC) return;

	UDamageTextWidget* DamageWidget = CreateWidget<UDamageTextWidget>(PC, DamageTextWidgetClass);
	DamageWidget->AddToViewport();
	FVector Dir = GetActorForwardVector();
	DamageWidget->SetTargetLocation(GetActorLocation() + FVector(Dir.X * -50, 0.0f, -50.0f));
	DamageWidget->SetTextHeal(Hp, Mp);
}

void AKeroroCharacter::ParryAttack()
{
	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	float Radius = 500.f;
	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		GetActorLocation(),
		GetActorLocation(),
		FQuat::Identity,
		ECC_GameTraceChannel3,
		FCollisionShape::MakeSphere(Radius),
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
				HitActor->TakeDamage(999.0f, DamageEvent, GetController(), this);
			}
		}
	}
}

void AKeroroCharacter::ResetGuardCooldown()
{
	CanGuarding = true;
	//UE_LOG(LogTemp, Log, TEXT("Guard cooldown finished!"));
}

void AKeroroCharacter::StartGuard()
{

	if (KRAnim)
	{
		if (KRAnim->bIsHit) return;
	}

	if (!KRStat)
	{
		return;
	}

	if (!CanGuarding)
	{
		UE_LOG(LogTemp, Warning, TEXT("Guard is on cooldown"));
		return;
	}

	// 콤보 공격 끝내기
	OnAttackMontageEnded(nullptr, false);
	KRAnim->StopAttackMontage();
	KRAnim->bIsGuarding = true;

	CanGuarding = false;
	IsGuarding = true;
	IsParrying = true;

	float ParryTime = KRStat->ParryTime;
	float GuardTime = KRStat->GuardTime;
	//float GuardTime = 10.0f;
	float GuardCoolTime = KRStat->GuardCoolTime;

	UE_LOG(LogTemp, Log, TEXT("StartGuard"));

	GetWorld()->GetTimerManager().SetTimer(ParryTimerHandle, this, &AKeroroCharacter::EndParry, ParryTime, false);
	GetWorld()->GetTimerManager().SetTimer(GuardTimerHandle, this, &AKeroroCharacter::EndGuard, GuardTime, false);
	GetWorld()->GetTimerManager().SetTimer(GuardCooldownTimer, this, &AKeroroCharacter::ResetGuardCooldown, GuardCoolTime, false);
	SpawnShieldEffect();
}

bool AKeroroCharacter::StartUltimateSkill()
{
	if (KRStat == nullptr) return false;

	float SkillCoolTimeCheck = GetRemainingUltimateSkillCooldown();
	if (SkillCoolTimeCheck != -1)return false;	// -1값이어야 타이머 안돌아감

	if (!KRStat->PayUlitiSkillMP())
	{
		//UE_LOG(LogTemp, Error, TEXT("Can't Play UlitiSkill"));
		return false;
	}

	float SkillCoolTime = KRStat->GetUlitiCoolTime(CurrentKeroroType);
	// 타이머핸들 이용하여 쿨타임 hud에넘겨줘서 쿨타임 위젯업데이트중
	GetWorld()->GetTimerManager().SetTimer(UltimateSkillCooldownTimer, []() {}, SkillCoolTime, false);

	// 궁극기 사운드 재생
	PlayUltiSkillSound();

	// 궁극기 시점 변경
	ChangeCameraUltimate();

	// 궁극기 로직 실행
	switch (WeaponType)
	{
	case EWeaponType::KEROBALL:
		KeroBallUlti();
		break;
	case EWeaponType::RIFLE:
		RifleUlti();
		break;
	case EWeaponType::SWORD:
		SwordUlti();
		break;
	case EWeaponType::FIST:
		FistUlti();
		break;
	case EWeaponType::NOTEBOOK:
		NoteBookUlti();
		break;
	}

	if (KRAnim)
	{
		//UE_LOG(LogTemp, Log, TEXT("StartUltimateSkill"));
		KRAnim->bIsUltiSkillPlaying = true;
		KRAnim->PlayUltiSkillMontage();
	}
	return true;
}

void AKeroroCharacter::EndParry()
{
	IsParrying = false;
	//UE_LOG(LogTemp, Log, TEXT("EndParry"));

}

void AKeroroCharacter::EndGuard()
{
	IsGuarding = false;
	IsParrying = false;
	KRAnim->bIsGuarding = false;
	//UE_LOG(LogTemp, Log, TEXT("EndGuard"));

	DestroyShieldEffect();
}

void AKeroroCharacter::SpawnShieldEffect()
{
	if (NCShieldEffect)
	{
		DestroyShieldEffect();
	}

	NCShieldEffect = UNiagaraFunctionLibrary::SpawnSystemAttached(
		NSShieldEffect,
		GetMesh(),
		NAME_None,
		FVector(0.0f, 100.f, 100.f),
		FRotator(0.0f, 90.0f, 0.0f),
		EAttachLocation::KeepRelativeOffset,
		true);

	UGameplayStatics::SpawnSoundAtLocation(this, ShieldSound, GetActorLocation(), FRotator::ZeroRotator, 1.0f);
}

void AKeroroCharacter::DestroyShieldEffect()
{
	if (NCShieldEffect)
	{
		NCShieldEffect->Deactivate();
		NCShieldEffect->DestroyComponent();
		NCShieldEffect = nullptr;
	}
}


void AKeroroCharacter::SpawnToHand()
{
	UE_LOG(LogTemp, Log, TEXT("SpawnToHand"));

	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = this;

	Weapon = GetWorld()->SpawnActor<AKeroballWeapon>(FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	TArray<FName> SocketNames = Weapon->GetSocketNames();
	if (SocketNames.IsValidIndex(0))
	{
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, SocketNames[0]);
	}
}

void AKeroroCharacter::ChangeFaceTexture(EKeroroType KeroroType, EFaceType FaceType)
{
	if (!FaceMaterialInstance) return;

	UKeroroGameInstance* GI = Cast<UKeroroGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!GI)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameInstance is null"));
		return;
	}

	FSoftObjectPath AssetPath = GI->GetFaceAssetPath(KeroroType, FaceType);
	if (!AssetPath.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid face asset path"));
		return;
	}

	// StreamableManager를 이용한 동기 로드
	UTexture2D* LoadedTexture = GI->GetStreamableManager().LoadSynchronous<UTexture2D>(AssetPath);
	if (LoadedTexture)
	{
		FaceMaterialInstance->SetTextureParameterValue(FName("FaceTexture"), LoadedTexture);
		UE_LOG(LogTemp, Log, TEXT("Face texture success"));
	}
}

void AKeroroCharacter::ChangeFaceTexture(EFaceType FaceType)
{
	if (!FaceMaterialInstance) return;

	int32 index = static_cast<int32>(FaceType);
	if (FaceTextures.IsValidIndex(index))
	{
		FaceMaterialInstance->SetTextureParameterValue(FName("FaceTexture"), FaceTextures[index]);
		//UE_LOG(LogTemp, Log, TEXT("Face texture success"));
	}
}

void AKeroroCharacter::StartRoll(FVector Dir, float RollDistance)
{
	// 방향 설정 (현재 바라보는 방향 또는 이동 중인 방향)
	FVector RollDirection = Dir; // 앞으로 구르기
	float RollDuration = 0.5f;

	FVector RollVelocity = RollDirection * (RollDistance / RollDuration);
	GetCharacterMovement()->BrakingFrictionFactor = 0.0f; // 구를 때 마찰 없애기
	LaunchCharacter(RollVelocity, true, true);

	if (KRAnim)
	{
		KRAnim->bIsRolling = true;
		KRAnim->PlayRollAnimation();
	}
}

void AKeroroCharacter::EndRoll()
{
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->BrakingFrictionFactor = 2.0f;
}

void AKeroroCharacter::StartRun()
{
	if (KRAnim)
	{
		if (KRAnim->bIsHit)
			return;
	}

	if (KRAnim != nullptr && KRStat != nullptr) {
		KRAnim->bIsRunning = true;
		GetCharacterMovement()->MaxWalkSpeed = KRStat->MaxMoveSpeed;
		//UE_LOG(LogTemp, Log, TEXT("Current Move Speed = %f"),GetCharacterMovement()->MaxWalkSpeed);
	}
}

void AKeroroCharacter::StopRun()
{
	if (KRAnim != nullptr && KRStat != nullptr) {
		KRAnim->bIsRunning = false;
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		//UE_LOG(LogTemp, Log, TEXT("Current Move Speed = %f"), GetCharacterMovement()->MaxWalkSpeed);
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
		WeaponType = EWeaponType::NOTEBOOK;
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
		KRAnim->SetKeroroType(type);
		// 애님인스턴스 설정 및 델리게이트 바인딩
		BindCharacterEvents();
		SetWeapon();

		// 표정 바꾸기 위한 동적 머테리얼 인스턴스 // 1번 슬롯 = 얼굴 표정
		UMaterialInterface* FaceMaterial = GetMesh()->GetMaterial(1);
		if (FaceMaterial)
		{
			FaceMaterialInstance = UMaterialInstanceDynamic::Create(FaceMaterial, this);
			GetMesh()->SetMaterial(1, FaceMaterialInstance);
		}
	}

	// 얼굴 텍스쳐 로드
	UKeroroGameInstance* GI = Cast<UKeroroGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!GI)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameInstance is null"));
		return;
	}

	FaceTextures.Reserve(static_cast<int32>(EFaceType::MAX));
	for (int32 i = 0; i < static_cast<int32>(EFaceType::MAX); ++i)
	{
		FSoftObjectPath AssetPath = GI->GetFaceAssetPath(CurrentKeroroType, static_cast<EFaceType>(i));

		if (AssetPath.IsValid())
		{
			UTexture2D* LoadedTexture = GI->GetStreamableManager().LoadSynchronous<UTexture2D>(AssetPath);
			if (LoadedTexture)
			{
				FaceTextures.Add(LoadedTexture);
			}
		}
	}

	// 캐릭터 공격 사운드 로드
	// 1부터 시작하는 이유 - rowname - KeroroAttackSound1처럼 1부터 시작함
	VoiceSounds.Reserve(NumVoices);
	for (int32 i = 1; i <= NumVoices; ++i)
	{
		FSoftObjectPath AssetPath = GI->GetVoiceSoundAssetPath(CurrentKeroroType, i);

		if (AssetPath.IsValid())
		{
			USoundBase* LoadedSound = GI->GetStreamableManager().LoadSynchronous<USoundBase>(AssetPath);
			if (LoadedSound)
			{
				VoiceSounds.Add(LoadedSound);
			}
		}
	}
}
