// Fill out your copyright notice in the Description page of Project Settings.


#include "KeroroPlayerController.h"
#include "KeroroCharacter.h"
#include "KeroroPlayerState.h"
#include "KeroroStatComponent.h"
#include "KeroroHUDWidget.h"
#include "KeroroEffectWidget.h"
#include "CutSceneWidget.h"
#include "KeroroGameState.h"
#include "KeroroAIController.h"
#include "NoteBookWeapon.h"
#include "LevelupCardWidget.h"
#include "StatusWidget.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "KeroroAnimInstance.h"
#include "KeroroGameMode.h"


AKeroroPlayerController::AKeroroPlayerController()
{
	// 입력
	LoadInputActionAndMappingContext();

	// 이펙트
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NS(TEXT("/Game/Knife_light/VFX/NE_attack05.NE_attack05"));
	if (NS.Succeeded())
	{
		NSTagEffect = NS.Object;
	}
	static ConstructorHelpers::FClassFinder<UKeroroHUDWidget> HUDCLASS(TEXT("/Game/Blueprints/KR_HUD_Widget.KR_HUD_Widget_C"));
	if (HUDCLASS.Succeeded())
	{
		KRHUDWidgetClass = HUDCLASS.Class;
	}
	static ConstructorHelpers::FClassFinder<UKeroroEffectWidget> PARRYWIDGET(TEXT("/Game/Blueprints/KR_Parrying_Effect_Widget.KR_Parrying_Effect_Widget_C"));
	if (PARRYWIDGET.Succeeded())
	{
		KRParryWidgetClass = PARRYWIDGET.Class;
	}
	static ConstructorHelpers::FClassFinder<UCameraShakeBase> CSB(TEXT("/Game/Blueprints/KR_Parrying_CameraShake.KR_Parrying_CameraShake_C"));
	if (CSB.Succeeded())
	{
		KRParryCameraShakeClass = CSB.Class;
	}
	static ConstructorHelpers::FClassFinder<UCameraShakeBase> ULTISHAKE(TEXT("/Game/Blueprints/KR_Ulti_Camera_Shake.KR_Ulti_Camera_Shake_C"));
	if (ULTISHAKE.Succeeded())
	{
		KRUltiSkillCameraShakeClass = ULTISHAKE.Class;
	}
	static ConstructorHelpers::FClassFinder<UCutSceneWidget> CUTSCENECLASS(TEXT("/Game/Blueprints/KR_CutSceneWidget.KR_CutSceneWidget_C"));
	if (CUTSCENECLASS.Succeeded())
	{
		KRCutSceneClass = CUTSCENECLASS.Class;
	}
	static ConstructorHelpers::FClassFinder<UStatusWidget>STATUSCLASS(TEXT("/Game/Blueprints/StatusWidget.StatusWidget_C"));
	if (STATUSCLASS.Succeeded())
	{
		KRStatusWidgetClass = STATUSCLASS.Class;
	}
	IsMagicCircleActivated = false;
}

void AKeroroPlayerController::OnPossess(APawn* PawnToPossess)
{
	Super::OnPossess(PawnToPossess);

}

void AKeroroPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AKeroroPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 플레이어스테이트 초기화
	KRPlayerState = Cast<AKeroroPlayerState>(PlayerState);

	// 포제스중인 캐릭터가져와서 캐스팅
	AKeroroCharacter* KRCharacter = Cast<AKeroroCharacter>(GetCharacter());
	if (KRCharacter == nullptr)return;


	// 캐릭터가 자신의 타입 알려주는 함수
	EKeroroType MyType = KRCharacter->GetKeroroCharacterType();
	FSpawnKero SpawnKero;
	SpawnKero.KeroroCharacter = KRCharacter;
	SpawnKero.bIsSpawnedOnce = true;
	CharacterMap.Add(MyType, SpawnKero); // TMap에 미리 등록


	// 현재 맵이름 가져옴 (로비면 hud ,캐릭터 머리위 hp바 히든으로 변경)
	FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);
	if (CurrentLevelName == TEXT("Robby1Level") || CurrentLevelName == TEXT("LoadingLevel"))
	{
		IsMainMap = false;
		KRCharacter->HiddenHPBarOnHead();
	}
	else {
		IsMainMap = true;
	}


	if (IsMainMap)
	{
		KRHUDWidget = CreateWidget<UKeroroHUDWidget>(this, KRHUDWidgetClass);
		if (KRHUDWidget == nullptr) return;

		KRHUDWidget->AddToViewport();
		KRHUDWidget->BindKRStat(KRCharacter->KRStat);
		KRHUDWidget->BindPlayerState(KRPlayerState);
		UpdateStatWidget();
	}

	KRCharacter->KRStat->OnHpIsChanged.AddUObject(this, &AKeroroPlayerController::UpdateHPWidget);
	KRCharacter->KRStat->OnMpIsChanged.AddUObject(this, &AKeroroPlayerController::UpdateMPWidget);
	KRPlayerState->OnLevelChanged.AddUObject(this, &AKeroroPlayerController::OnPlayerLevelUpdated);
}

void AKeroroPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (KRPlayerState)
	{
		KRPlayerState->OnLevelChanged.RemoveAll(this);
	}

	if (AKeroroCharacter* KRCharacter = Cast<AKeroroCharacter>(GetCharacter()))
	{
		if (KRCharacter->KRStat)
		{
			KRCharacter->KRStat->OnHpIsChanged.RemoveAll(this);
			KRCharacter->KRStat->OnMpIsChanged.RemoveAll(this);
		}
	}

	Super::EndPlay(EndPlayReason);
}

void AKeroroPlayerController::UpdateStatCardEnhanced()
{

	AKeroroPlayerState* PS = GetPlayerState<AKeroroPlayerState>();
	if (!PS) {
		UE_LOG(LogTemp, Error, TEXT("PS in Playercontrollerr nullptr"));
		return;
	}

	AKeroroCharacter* Kero = Cast<AKeroroCharacter>(GetCharacter());
	if (Kero)
	{
		if (Kero->KRStat)
		{
			Kero->KRStat->UpdateStatCardEnhanced(PS);
		}
	}
}

// 처음초기화용
void AKeroroPlayerController::UpdateStatWidget()
{
	if (IsValid(KRHUDWidget))
	{
		KRHUDWidget->UpdateHPWidget();
		KRHUDWidget->UpdateLevelWidget();
		KRHUDWidget->UpdateEXPWidget();
		KRHUDWidget->UpdateGoldWidget();
	}
}

void AKeroroPlayerController::UpdateHPWidget()
{
	if (IsValid(KRHUDWidget))
	{
		KRHUDWidget->UpdateHPWidget();
	}
}

void AKeroroPlayerController::UpdateMPWidget()
{
	if (IsValid(KRHUDWidget))
	{
		KRHUDWidget->UpdateMPWidget();
	}
}

void AKeroroPlayerController::UpdateGoldWidget()
{
	if (IsValid(KRHUDWidget))
	{
		KRHUDWidget->UpdateGoldWidget();
	}
}

void AKeroroPlayerController::UpdateLevelWidget()
{
	if (IsValid(KRHUDWidget))
	{
		KRHUDWidget->UpdateLevelWidget();
	}
}

void AKeroroPlayerController::UpdateTimeWidget(float RemainTime)
{
	if (IsValid(KRHUDWidget))
	{
		KRHUDWidget->UpdateTimeWidget(GetGameStateRemainingTime());
	}
}

void AKeroroPlayerController::UpdateEXPWidget()
{
}

void AKeroroPlayerController::UpdateKillWidget()
{
}

void AKeroroPlayerController::PlayUltimateCutScene()
{
	if (!KRCutSceneClass) return;
	KRUltimateCutSceneWidget = CreateWidget<UCutSceneWidget>(this, KRCutSceneClass);
	if (KRUltimateCutSceneWidget)
	{
		KRUltimateCutSceneWidget->AddToViewport(101);
		KRUltimateCutSceneWidget->PlayCutscene();
	}
}

void AKeroroPlayerController::PlayParryWidgetEffect()
{
	if (!KRParryWidgetClass) return;

	KRParryWidget = CreateWidget<UKeroroEffectWidget>(this, KRParryWidgetClass);
	if (KRParryWidget)
	{
		KRParryWidget->AddToViewport(100);
		KRParryWidget->PlayParryWidgetEffect();


		FTimerHandle RemoveHandle;
		GetWorld()->GetTimerManager().SetTimer(RemoveHandle, [this]()
			{
				if (KRParryWidget)
				{
					KRParryWidget->RemoveFromParent();
					KRParryWidget = nullptr;
				}
			}, 0.4f, false);
	}
}

void AKeroroPlayerController::PlayParryCameraShake()
{
	PlayerCameraManager->StartCameraShake(KRParryCameraShakeClass, 1.0f);
	//UE_LOG(LogTemp, Warning, TEXT("PlayParryCameraShake"));

}

void AKeroroPlayerController::PlayUltiSkillCameraShake()
{
	PlayerCameraManager->StartCameraShake(KRUltiSkillCameraShakeClass, 1.0f);
	//UE_LOG(LogTemp, Warning, TEXT("PlayUltiSkillCameraShake"));
}

void AKeroroPlayerController::ShowStatusWidget()
{
	if (!KRStatusWidgetClass) return;
	if (!IsMainMap) return;
	KRStatusWidget = CreateWidget<UStatusWidget>(this, KRStatusWidgetClass);
	if (KRStatusWidget)
	{
		KRStatusWidget->AddToViewport(101);
	}
	SetUIMode();
}

void AKeroroPlayerController::UseItemSlotZ()
{
	if (!KRHUDWidget) return;
	KRHUDWidget->UseItem(1);
}

void AKeroroPlayerController::UseItemSlotX()
{
	if (!KRHUDWidget) return;
	KRHUDWidget->UseItem(2);
}

void AKeroroPlayerController::UseItemSlotC()
{
	if (!KRHUDWidget) return;
	KRHUDWidget->UseItem(3);
}

float AKeroroPlayerController::GetGameStateRemainingTime()
{
	if (GetWorld())
	{
		AKeroroGameState* GameState = GetWorld()->GetGameState<AKeroroGameState>();
		if (GameState)
		{
			return GameState->GetRemainingTime();
		}
	}
	return 0.0f;
}

void AKeroroPlayerController::TagCharacter(EKeroroType TargetType)
{
	if (!KRPlayerState || !IsMainMap) return;

	// 아직 해금되지 않은 캐릭터면 return
	if (!KRPlayerState->IsCharacterUnlocked(TargetType))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is locked!"), *UEnum::GetValueAsString(TargetType));
		return;
	}

	// 1. 기존 캐릭터 델리게이트 해제
	AKeroroCharacter* PrevCharacter = Cast<AKeroroCharacter>(GetCharacter());
	if (PrevCharacter && PrevCharacter->KRStat)
	{
		PrevCharacter->KRStat->OnHpIsChanged.RemoveAll(this);
		PrevCharacter->KRStat->OnMpIsChanged.RemoveAll(this);
	}

	AKeroroCharacter* NewCharacter = nullptr;

	// 2. 이미 존재하는 캐릭터 있는지 확인
	if (CharacterMap.Contains(TargetType) && IsValid(CharacterMap[TargetType].KeroroCharacter))
	{
		NewCharacter = CharacterMap[TargetType].KeroroCharacter;
	}
	else if (CharacterMap.Contains(TargetType) && CharacterMap[TargetType].KeroroCharacter == nullptr && CharacterMap[TargetType].bIsSpawnedOnce == true)
	{
		// 이미 생성됐었고 죽었으면 더이상 스폰 x
		return;
	}
	else
	{
		FVector SpawnLoc;
		FRotator SpawnRot;

		if (PrevCharacter == nullptr)
		{
			SpawnLoc = FVector::ZeroVector;
			SpawnRot = FRotator::ZeroRotator;
		}
		else
		{
			SpawnLoc = PrevCharacter->GetActorLocation() + FVector(0, 0, 300);
			SpawnRot = PrevCharacter->GetActorRotation();
		}

		FTransform SpawnTransform = FTransform(SpawnRot, SpawnLoc);

		NewCharacter = GetWorld()->SpawnActorDeferred<AKeroroCharacter>(
			AKeroroCharacter::StaticClass(), SpawnTransform, this, nullptr);

		if (NewCharacter)
		{
			NewCharacter->CurrentKeroroType = TargetType;
			UGameplayStatics::FinishSpawningActor(NewCharacter, SpawnTransform);
			FSpawnKero SpawnKero;
			SpawnKero.KeroroCharacter = NewCharacter;
			SpawnKero.bIsSpawnedOnce = true;

			CharacterMap.Add(TargetType, SpawnKero);
		}
	}

	// 2.5 플레이어스테이트에 현재 케로로 타입 저장
	KRPlayerState->SetCharacterType(TargetType);

	// 3. 기존 캐릭터를 AIController로 변경
	if (PrevCharacter)
	{
		AKeroroAIController* AIController = GetWorld()->SpawnActor<AKeroroAIController>(AKeroroAIController::StaticClass());
		if (AIController)
		{
			AIController->Possess(PrevCharacter);
		}
	}

	// 4. 새 캐릭터를 플레이어컨트롤러가 소유
	Possess(NewCharacter);

	// 5. 새 캐릭터 델리게이트 바인딩 및 HUD 갱신
	if (KRHUDWidget && NewCharacter)
	{
		NewCharacter->KRStat->OnHpIsChanged.AddUObject(this, &AKeroroPlayerController::UpdateHPWidget);
		NewCharacter->KRStat->OnMpIsChanged.AddUObject(this, &AKeroroPlayerController::UpdateMPWidget);

		KRHUDWidget->BindKRStat(NewCharacter->KRStat);
		KRHUDWidget->ChangeSkillImage();
	}

	// 6. 태그 이펙트 재생
	if (NSTagEffect && GetCharacter() != nullptr)
	{
		FVector EffectLoc = GetCharacter()->GetActorLocation() + GetCharacter()->GetActorForwardVector() * 100.0f;
		FRotator EffectRot = GetCharacter()->GetActorRotation();

		NCTagEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NSTagEffect, EffectLoc, EffectRot, FVector(1.0f));
	}
}

void AKeroroPlayerController::Guard()
{
	AKeroroCharacter* Kero = Cast<AKeroroCharacter>(GetCharacter());
	if (Kero)
	{
		Kero->StartGuard();
	}
}

void AKeroroPlayerController::UltimateSkill()
{
	AKeroroCharacter* Kero = Cast<AKeroroCharacter>(GetCharacter());
	if (Kero)
	{
		if (Kero->StartUltimateSkill())
		{
			PlayUltimateCutScene();
			PlayUltiSkillCameraShake();
		}
	}
}


void AKeroroPlayerController::Die()
{
	EKeroroType CurrentKeroroType = KRPlayerState->GetCurrentCharacterType();

	UnPossess();
	if (CharacterMap.Contains(CurrentKeroroType))
	{
		CharacterMap[CurrentKeroroType].KeroroCharacter = nullptr;
	}

	// 다음 캐릭터 탐색 (순환)
	int Max = static_cast<int32>(EKeroroType::MAX); // enum 마지막 값
	for (int i = 1; i < Max; ++i)
	{
		EKeroroType NextType = static_cast<EKeroroType>(i);

		if (CharacterMap.Contains(NextType))
		{
			AKeroroCharacter* NextCharacter = CharacterMap[NextType].KeroroCharacter;
			if (IsValid(NextCharacter))
			{
				Possess(NextCharacter);
				KRPlayerState->SetCurrentCharacterType(NextType);
				KRHUDWidget->BindKRStat(NextCharacter->KRStat);
				return;
			}
		}
	}

	if (AKeroroGameMode* GM = GetWorld()->GetAuthGameMode<AKeroroGameMode>())
	{
		GM->EndGame(false);
	}
}

void AKeroroPlayerController::DieAIKero(EKeroroType type)
{
	// bIsSpawnedOnce는 사실 안해도됨 태그캐릭터에서 하고있으니
	// 캐릭터 = 널포인트 , bIsSpawnedOnce = true면 스폰됐으나 죽었다는 뜻
	CharacterMap[type].KeroroCharacter = nullptr;
	CharacterMap[type].bIsSpawnedOnce = true;
}

void AKeroroPlayerController::SetUIMode()
{
	if (!KRHUDWidget) return;
	GetWorld()->GetWorldSettings()->SetTimeDilation(0.1f);

	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(KRHUDWidget->TakeWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputMode);

	bShowMouseCursor = true;
}



void AKeroroPlayerController::SetGameMode()
{
	//SetPause(false);
	GetWorld()->GetWorldSettings()->SetTimeDilation(1.0f);

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	bShowMouseCursor = false;
}




// 플레이어 스테이트에서 레벨업할시 델리게이트에의해 호출하여
// 관리중인 모든 캐릭터 레벨 초기화
void AKeroroPlayerController::OnPlayerLevelUpdated()
{
	int32 Level = KRPlayerState->CurrentLevel;

	for (auto& pair : CharacterMap)
	{
		AKeroroCharacter* kero = pair.Value.KeroroCharacter;
		if (kero && kero->KRStat)
		{
			kero->KRStat->SetLevel(Level, KRPlayerState);

		}
	}
}

void AKeroroPlayerController::OnMagicCircleActivated()
{
	IsMagicCircleActivated = !IsMagicCircleActivated;
	AKeroroCharacter* Kero = Cast<AKeroroCharacter>(GetCharacter());
	if (Kero)
	{
		if (Kero->WeaponType == EWeaponType::NOTEBOOK)
		{
			if (IsMagicCircleActivated == true) {
				Kero->ChangeCameraNoteBookAttack();
			}
			else {
				Kero->ChangeCameraDefault();
			}
		}
	}
}


void AKeroroPlayerController::LoadInputActionAndMappingContext()
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC_KERORO(TEXT("/Game/Input/IMC_Keroro.IMC_Keroro"));
	if (IMC_KERORO.Succeeded())InputMappingContext = IMC_KERORO.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_MOVE(TEXT("/Game/Input/IA_Keroro_Move.IA_Keroro_Move"));
	if (IA_MOVE.Succeeded()) Moving = IA_MOVE.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_LOOK(TEXT("/Game/Input/IA_Keroro_Look.IA_Keroro_Look"));
	if (IA_LOOK.Succeeded()) Looking = IA_LOOK.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_JUMP(TEXT("/Game/Input/IA_Keroro_Jump.IA_Keroro_Jump"));
	if (IA_JUMP.Succeeded()) Jumping = IA_JUMP.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_ROLL(TEXT("/Game/Input/IA_Keroro_Roll.IA_Keroro_Roll"));
	if (IA_ROLL.Succeeded()) RollingForward = IA_ROLL.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_RUN(TEXT("/Game/Input/IA_Keroro_Run.IA_Keroro_Run"));
	if (IA_RUN.Succeeded()) Running = IA_RUN.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_ATTACK(TEXT("/Game/Input/IA_Keroro_Attack.IA_Keroro_Attack"));
	if (IA_ATTACK.Succeeded()) Attacking = IA_ATTACK.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_MOUSE_RIGHT(TEXT("/Game/Input/IA_MOUSE_RIGHT_BUTTON.IA_MOUSE_RIGHT_BUTTON"));
	if (IA_MOUSE_RIGHT.Succeeded()) MouseRight = IA_MOUSE_RIGHT.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_GUARD(TEXT("/Game/Input/IA_Keroro_Guard.IA_Keroro_Guard"));
	if (IA_GUARD.Succeeded()) Guarding = IA_GUARD.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_SKILL(TEXT("/Game/Input/IA_Keroro_Ultimate_Skill.IA_Keroro_Ultimate_Skill"));
	if (IA_SKILL.Succeeded()) SkillAction = IA_SKILL.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_STATUS(TEXT("/Game/Input/IA_Keroro_Status.IA_Keroro_Status"));
	if (IA_STATUS.Succeeded()) ShowStatus = IA_STATUS.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_NUM1(TEXT("/Game/Input/IA_NUM1.IA_NUM1"));
	if (IA_NUM1.Succeeded()) Num1 = IA_NUM1.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_NUM2(TEXT("/Game/Input/IA_NUM2.IA_NUM2"));
	if (IA_NUM2.Succeeded()) Num2 = IA_NUM2.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_NUM3(TEXT("/Game/Input/IA_NUM3.IA_NUM3"));
	if (IA_NUM3.Succeeded()) Num3 = IA_NUM3.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_NUM4(TEXT("/Game/Input/IA_NUM4.IA_NUM4"));
	if (IA_NUM4.Succeeded()) Num4 = IA_NUM4.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_NUM5(TEXT("/Game/Input/IA_NUM5.IA_NUM5"));
	if (IA_NUM5.Succeeded()) Num5 = IA_NUM5.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_ITEMZ(TEXT("/Game/Input/IA_ITEMZ.IA_ITEMZ"));
	if (IA_ITEMZ.Succeeded()) ItemZ = IA_ITEMZ.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_ITEMX(TEXT("/Game/Input/IA_ITEMX.IA_ITEMX"));
	if (IA_ITEMX.Succeeded()) ItemX = IA_ITEMX.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_ITEMC(TEXT("/Game/Input/IA_ITEMC.IA_ITEMC"));
	if (IA_ITEMC.Succeeded()) ItemC = IA_ITEMC.Object;
}



void AKeroroPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}

	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(InputComponent))
	{
		Input->BindAction(Moving, ETriggerEvent::Triggered, this, &AKeroroPlayerController::Move);
		Input->BindAction(Looking, ETriggerEvent::Triggered, this, &AKeroroPlayerController::Look);
		Input->BindAction(Jumping, ETriggerEvent::Triggered, this, &AKeroroPlayerController::Jump);
		Input->BindAction(RollingForward, ETriggerEvent::Started, this, &AKeroroPlayerController::Roll);
		Input->BindAction(Running, ETriggerEvent::Triggered, this, &AKeroroPlayerController::StartRun);
		Input->BindAction(Running, ETriggerEvent::Completed, this, &AKeroroPlayerController::StopRun);
		Input->BindAction(Attacking, ETriggerEvent::Started, this, &AKeroroPlayerController::Attack);
		Input->BindAction(MouseRight, ETriggerEvent::Started, this, &AKeroroPlayerController::OnMagicCircleActivated);
		Input->BindAction(Guarding, ETriggerEvent::Started, this, &AKeroroPlayerController::Guard);
		Input->BindAction(SkillAction, ETriggerEvent::Started, this, &AKeroroPlayerController::UltimateSkill);
		Input->BindAction(ShowStatus, ETriggerEvent::Started, this, &AKeroroPlayerController::ShowStatusWidget);
		Input->BindAction(ItemZ, ETriggerEvent::Started, this, &AKeroroPlayerController::UseItemSlotZ);
		Input->BindAction(ItemX, ETriggerEvent::Started, this, &AKeroroPlayerController::UseItemSlotX);
		Input->BindAction(ItemC, ETriggerEvent::Started, this, &AKeroroPlayerController::UseItemSlotC);
		Input->BindAction(Num1, ETriggerEvent::Started, this, &AKeroroPlayerController::TagKeroro);
		Input->BindAction(Num2, ETriggerEvent::Started, this, &AKeroroPlayerController::TagTamama);
		Input->BindAction(Num3, ETriggerEvent::Started, this, &AKeroroPlayerController::TagGiroro);
		Input->BindAction(Num4, ETriggerEvent::Started, this, &AKeroroPlayerController::TagDororo);
		Input->BindAction(Num5, ETriggerEvent::Started, this, &AKeroroPlayerController::TagKururu);
	}
}

void AKeroroPlayerController::Move(const FInputActionValue& Value)
{
	if (IsMagicCircleActivated) {
		AKeroroCharacter* kero = Cast<AKeroroCharacter>(GetCharacter());
		if (kero->WeaponType == EWeaponType::NOTEBOOK) return;
	}

	if (AKeroroCharacter* kero = Cast<AKeroroCharacter>(GetCharacter()))
	{
		if (!kero->KRAnim)return;
		if (kero->KRAnim->bIsRolling) return;
		const FVector2D InputVector = Value.Get<FVector2D>();
		const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);

		const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		kero->AddMovementInput(Forward, InputVector.Y);
		kero->AddMovementInput(Right, InputVector.X);
	}
}

void AKeroroPlayerController::Look(const FInputActionValue& Value)
{

	const FVector2D LookInput = Value.Get<FVector2D>();

	if (IsMagicCircleActivated)
	{
		AKeroroCharacter* kero = Cast<AKeroroCharacter>(GetCharacter());
		if (kero && kero->WeaponType == EWeaponType::NOTEBOOK)
		{
			Cast<ANoteBookWeapon>(kero->Weapon)->ActivateMagicCircle();
			//UE_LOG(LogTemp, Warning, TEXT("Activate"));
		}
	}
	else
	{
		AKeroroCharacter* kero = Cast<AKeroroCharacter>(GetCharacter());
		if (kero && kero->WeaponType == EWeaponType::NOTEBOOK)
		{
			Cast<ANoteBookWeapon>(kero->Weapon)->DeactivateMagicCircle2();
			//UE_LOG(LogTemp, Warning, TEXT("DeActivate"));
		}
	}

	AddYawInput(LookInput.X);
	AddPitchInput(LookInput.Y);
}

void AKeroroPlayerController::Roll(const struct FInputActionValue& Value)
{
	if (AKeroroCharacter* kero = Cast<AKeroroCharacter>(GetCharacter()))
	{
		UKeroroAnimInstance* keroAnim = kero->KRAnim;
		if (!keroAnim)return;
		if (keroAnim->bIsRolling || keroAnim->bIsHit) return;

		const FVector2D InputVector = Value.Get<FVector2D>();
		const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);
		const FVector ForwardDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		FVector RollDirection = (ForwardDir * InputVector.Y + RightDir * InputVector.X).GetSafeNormal();
		kero->SetActorRotation(RollDirection.Rotation());

		kero->StartRoll(RollDirection, 900.0f);
	}
}

void AKeroroPlayerController::Jump()
{
	if (AKeroroCharacter* kero = Cast<AKeroroCharacter>(GetCharacter()))
	{
		kero->Jump();
	}

	//// test
	//if (KRHUDWidget)
	//{
	//	KRHUDWidget->PlayDrawAnimation_AllCard();
	//}
}

void AKeroroPlayerController::StartRun()
{
	if (AKeroroCharacter* kero = Cast<AKeroroCharacter>(GetCharacter()))
	{
		kero->StartRun();
	}
}

void AKeroroPlayerController::StopRun()
{
	if (AKeroroCharacter* kero = Cast<AKeroroCharacter>(GetCharacter()))
	{
		kero->StopRun();
	}
}

void AKeroroPlayerController::Attack()
{
	AKeroroCharacter* kero = Cast<AKeroroCharacter>(GetCharacter());
	if (IsValid(kero))
	{
		kero->Attack();
	}
}

void AKeroroPlayerController::TagKeroro()
{
	TagCharacter(EKeroroType::Keroro);
}

void AKeroroPlayerController::TagTamama()
{
	TagCharacter(EKeroroType::Tamama);
}

void AKeroroPlayerController::TagGiroro()
{
	TagCharacter(EKeroroType::Giroro);
}

void AKeroroPlayerController::TagDororo()
{
	TagCharacter(EKeroroType::Dororo);
}

void AKeroroPlayerController::TagKururu()
{
	TagCharacter(EKeroroType::Kururu);
}


