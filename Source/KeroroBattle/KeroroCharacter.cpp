// Fill out your copyright notice in the Description page of Project Settings.


#include "KeroroCharacter.h"
#include "KeroroEnemyCharacter.h"
#include "KeroroPlayerController.h"
#include "KeroroAnimInstance.h"
#include "KeroroWeapon.h"
#include "KeroroGameInstance.h"
#include "KeroballWeapon.h"
#include "SwordWeapon.h"
#include "FistWeapon.h"
#include "RifleWeapon.h"
#include "NoteBookWeapon.h"
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
#include "Engine/StreamableManager.h"
#include "Kismet/GameplayStatics.h"


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
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NE3(TEXT("/Game/Vefects/Zap_VFX/VFX/Zap/Particles/NS_Zap_03_Yellow.NS_Zap_03_Yellow"));
	if (NE3.Succeeded())
	{
		NSFistHitEffect = NE3.Object;
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
			KRStat->StartHeal();
		}

	}
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
		//UE_LOG(LogTemp, Warning, TEXT("Bind KRStat Succeced"));
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
	if (KRStat == nullptr) return 0.0f;

	float RandEvasion = FMath::FRand();

	if (RandEvasion < KRStat->EvasionRate) {
		UE_LOG(LogTemp, Error, TEXT("evasion attack~~"));
		return 0.0f;
	}

	float FinalDamage = Damage * (1.0f - KRStat->DefenseRate);
	//UE_LOG(LogTemp, Error, TEXT("in damage = %f, final Damage = %f"), Damage, FinalDamage);
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
	//if (WeaponType == EWeaponType::NOTEBOOK)
	//{
	//	Cast<ANoteBookWeapon>(Weapon)->ActivateFinalEffect();
	//	KRAnim->PlayAttackMontage();
	//}

	if (IsAttacking) // 애니메이션(몽타주) 재생중인가
	{
		HandleComboInput();
	}
	else
	{
		StartNewAttack();
	}
	int RandomIndex = FMath::RandRange(0, 5);
	ChangeFaceTexture(static_cast<EFaceType>(RandomIndex));
}

void AKeroroCharacter::HandleComboInput()
{
	if (!FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo)) return;
	if (CanNextCombo)IsComboInputOn = true;
}

void AKeroroCharacter::StartNewAttack()
{
	if (CurrentCombo != 0) return;

	// 공격 시작시 컨트롤러 방향으로 캐릭터 회전
	if (WeaponType == EWeaponType::RIFLE || WeaponType == EWeaponType::KEROBALL)
	{
		FRotator ControlRotation = GetControlRotation();
		ControlRotation.Pitch = 0.0f;
		ControlRotation.Roll = 0.0f;
		SetActorRotation(ControlRotation);
		GetCharacterMovement()->bOrientRotationToMovement = false; // 입력 방향 따라 몸 회전 안되게
	}

	AttackStartComboState();
	KRAnim->PlayAttackMontage();
	KRAnim->JumptoAttackMontageSection(CurrentCombo);
	IsAttacking = true;
}

void AKeroroCharacter::PlayVoiceSound()
{
	if (Cast<AKeroroPlayerController>(GetController()))
	{
		if (WeaponType == EWeaponType::RIFLE)
		{
			int RandomIndex = FMath::RandRange(0, 10);	//0~3 유효 4~10 무효 , 무효시 아무소리안나게 너무시끄러움..
			if (VoiceSounds.IsValidIndex(RandomIndex))
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

void AKeroroCharacter::SetWeapon()
{
	if (Weapon)
	{
		Weapon->Destroy();
	}

	switch (WeaponType)
	{
	case EWeaponType::FIST:
		Weapon = GetWorld()->SpawnActor<AFistWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
		Cast<AFistWeapon>(Weapon)->InitEffect(this);
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
	case EWeaponType::NOTEBOOK:
		Weapon = GetWorld()->SpawnActor<ANoteBookWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
		Cast<ANoteBookWeapon>(Weapon)->InitEffect(this);
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
		//UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NSSWordEffect,
		//	GetActorLocation() + GetActorForwardVector() * 100.0f, // 캐릭터 앞 방향으로 100 유닛 이동
		//	GetActorRotation(),
		//	FVector(2.0f)
		//);

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
		KRAnim->OnEffectCreateCheck.AddUObject(this, &AKeroroCharacter::PlayEffect);

		// 공격 충돌 체크 바인딩
		KRAnim->OnAttackHitCheck.AddUObject(this, &AKeroroCharacter::AttackCheck);

		// 캐릭터 보이스 체크 바인딩
		KRAnim->OnVoiceCheck.AddUObject(this, &AKeroroCharacter::PlayVoiceSound);

		// 무기 다시생성
		if (WeaponType == EWeaponType::KEROBALL)
		{
			KRAnim->OnNextAttackCheck.AddUObject(this, &AKeroroCharacter::SpawnToHand);
		}

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
	GetCharacterMovement()->bOrientRotationToMovement = true; // 입력 방향 따라 몸 회전
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
				FDamageEvent DamageEvent;
				HitActor->TakeDamage(KRStat->AttackPower, DamageEvent, GetController(), this);
				//UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *HitActor->GetName());
			}
		}
		if (KRStat)
		{
			KRStat->AttackHeal();
		}
	}

	//if (KRStat) {
	//	float a = KRStat->MaxHp;
	//	float b = KRStat->CurrentHp;
	//	UE_LOG(LogTemp, Warning, TEXT("=========================CURRENT HP = %f============================"), b);
	//	UE_LOG(LogTemp, Warning, TEXT("=========================MAX HP = %f============================"), a);
	//}
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
	float AddYaw = (BulletNum > 1) ? (MaxAngle * 2.0f) / (BulletNum - 1) : 0.0f;

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
		MuzzleRotation.Pitch = 0.0f;
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
			FDamageEvent DamageEvent;
			HitActor->TakeDamage(KRStat->AttackPower, DamageEvent, GetController(), this);
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				NSFistHitEffect,
				HitResult.ImpactPoint,
				HitResult.ImpactNormal.Rotation(),
				FVector(1.0f)
			);
			Weapon->PlaySound(CurrentCombo);

		}
		if (KRStat)
		{
			KRStat->AttackHeal();
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

void AKeroroCharacter::SpawnToHand()
{
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
