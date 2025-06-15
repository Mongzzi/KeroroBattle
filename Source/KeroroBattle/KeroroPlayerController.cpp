// Fill out your copyright notice in the Description page of Project Settings.


#include "KeroroPlayerController.h"
#include "KeroroCharacter.h"
#include "KeroroPlayerState.h"
#include "KeroroStatComponent.h"
#include "KeroroHUDWidget.h"
#include "KeroroGameState.h"
#include "KeroroAIController.h"
#include "NoteBookWeapon.h"
#include "LevelupCardWidget.h"
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
	static ConstructorHelpers::FClassFinder<UKeroroHUDWidget> WidgetClassFinder(TEXT("/Game/Blueprints/KR_HUD_Widget.KR_HUD_Widget_C"));
	if (WidgetClassFinder.Succeeded())
	{
		KRHUDWidgetClass = WidgetClassFinder.Class;
	}
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
	CharacterMap.Add(MyType, KRCharacter); // TMap에 미리 등록

	KRHUDWidget = CreateWidget<UKeroroHUDWidget>(this, KRHUDWidgetClass);
	if (KRHUDWidget == nullptr) return;

	KRHUDWidget->AddToViewport();
	KRHUDWidget->BindKRStat(KRCharacter->KRStat);
	KRHUDWidget->BindPlayerState(KRPlayerState);


	KRCharacter->KRStat->OnHpIsChanged.AddUObject(this, &AKeroroPlayerController::UpdateHPWidget);
	KRPlayerState->OnLevelChanged.AddUObject(this, &AKeroroPlayerController::OnPlayerLevelUpdated);
	UpdateStatWidget();
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
	KRHUDWidget->UpdateTimeWidget(GetGameStateRemainingTime());
}

void AKeroroPlayerController::UpdateEXPWidget()
{
}

void AKeroroPlayerController::UpdateKillWidget()
{
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

void AKeroroPlayerController::TagCharacter()
{
	if (!KRPlayerState) return;

	// 다음 캐릭터 타입
	EKeroroType NextType = KRPlayerState->SetNextCharacterType();
	AKeroroCharacter* NewCharacter = nullptr;

	// 이미 존재하는 캐릭터가 있는지 확인
	if (CharacterMap.Contains(NextType) && IsValid(CharacterMap[NextType]))
	{
		UE_LOG(LogTemp, Warning, TEXT("character is in world"));
		NewCharacter = CharacterMap[NextType];
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("spawn new character"));
		FVector SpawnLoc;
		FRotator SpawnRot;

		// 캐릭터 전혀없을때
		if (GetCharacter() == nullptr)
		{
			SpawnLoc = FVector(0.0f, 0.0f, 0.0f);
			SpawnRot = FRotator::ZeroRotator;
		}
		else
		{
			SpawnLoc = GetCharacter()->GetActorLocation() + FVector(0, 0, 300);
			SpawnRot = GetCharacter()->GetActorRotation();
		}
		FTransform SpawnTransform = FTransform(SpawnRot, SpawnLoc);

		NewCharacter = GetWorld()->SpawnActorDeferred<AKeroroCharacter>(AKeroroCharacter::StaticClass(), SpawnTransform, this, nullptr);
		if (NewCharacter)
		{
			NewCharacter->CurrentKeroroType = NextType;
			UGameplayStatics::FinishSpawningActor(NewCharacter, SpawnTransform);

			NewCharacter->KRStat->SetLevel(KRPlayerState->CurrentLevel,KRPlayerState);
			CharacterMap.Add(NextType, NewCharacter);
		}
	}
	//--------------------------------------------------------------------------------------------------------
	// AIController 할당
	if (GetCharacter())
	{
		AKeroroCharacter* PreCharacter = Cast<AKeroroCharacter>(GetCharacter());
		AKeroroAIController* AIController = GetWorld()->SpawnActor<AKeroroAIController>(AKeroroAIController::StaticClass());

		if (AIController)
		{
			AIController->Possess(PreCharacter);
		}
	}
	Possess(NewCharacter);
	//--------------------------------------------------------------------------------------------------------
	// 캐릭터 스탯컴포넌트 HUD에 바인딩 및 체력 업데이트
	if (KRHUDWidget && NewCharacter)
	{
		// 새로운 캐릭터가 태그되었을 때, 해당 캐릭터의 HP 델리게이트를 바인딩
		NewCharacter->KRStat->OnHpIsChanged.AddUObject(this, &AKeroroPlayerController::UpdateHPWidget);

		// 새로운 캐릭터에 대한 스탯 바인딩
		// 초기 HP 값,레벨 갱신
		KRHUDWidget->BindKRStat(NewCharacter->KRStat);

		//UpdateStatWidget();
	}
	//--------------------------------------------------------------------------------------------------------
	// 태그 이펙트
	if (NSTagEffect && GetCharacter() != nullptr)
	{
		FVector EffectLoc = GetCharacter()->GetActorLocation() + GetCharacter()->GetActorForwardVector() * 100.0f;
		FRotator EffecRot = GetCharacter()->GetActorRotation();

		NCTagEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NSTagEffect, EffectLoc, EffecRot, FVector(1.0f));
	}
}


void AKeroroPlayerController::Die()
{
	EKeroroType CurrentKeroroType = KRPlayerState->GetCurrentCharacterType();

	UnPossess();
	if (CharacterMap.Contains(CurrentKeroroType))
	{
		CharacterMap.Remove(CurrentKeroroType);
	}

	// 다음 캐릭터 탐색 (순환)
	int Max = static_cast<int32>(EKeroroType::MAX); // enum 마지막 값
	for (int i = 1; i < Max; ++i)
	{
		EKeroroType NextType = static_cast<EKeroroType>((static_cast<int>(CurrentKeroroType) + i) % Max);

		if (CharacterMap.Contains(NextType))
		{
			AKeroroCharacter* NextCharacter = CharacterMap[NextType];
			if (IsValid(NextCharacter))
			{
				Possess(NextCharacter);
				KRPlayerState->SetCurrentCharacterType(NextType);
				KRHUDWidget->BindKRStat(NextCharacter->KRStat);
				//UpdateStatWidget();
				return;
			}
		}
	}

}

void AKeroroPlayerController::SetUIMode()
{
	// test card draw
	FInputModeUIOnly InputMode;
	SetInputMode(InputMode);
	bShowMouseCursor = true;
	SetPause(true);
}

void AKeroroPlayerController::SetGameMode()
{
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
	bShowMouseCursor = false;
	SetPause(false);
}


// 플레이어 스테이트에서 레벨업할시 델리게이트에의해 호출하여
// 관리중인 모든 캐릭터 레벨 초기화
void AKeroroPlayerController::OnPlayerLevelUpdated()
{
	int32 Level = KRPlayerState->CurrentLevel;

	for (auto& pair : CharacterMap)
	{
		AKeroroCharacter* kero = pair.Value;
		if (kero)
		{
			if (kero->KRStat)
			{
				kero->KRStat->SetLevel(Level);
			}
		}
	}
}

void AKeroroPlayerController::OnMagicCircleActivated()
{
	IsMagicCircleActivated = !IsMagicCircleActivated;
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

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_RUN(TEXT("/Game/Input/IA_Keroro_Run.IA_Keroro_Run"));
	if (IA_RUN.Succeeded()) Running = IA_RUN.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_ATTACK(TEXT("/Game/Input/IA_Keroro_Attack.IA_Keroro_Attack"));
	if (IA_ATTACK.Succeeded()) Attacking = IA_ATTACK.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_TAG(TEXT("/Game/Input/IA_Keroro_Tag.IA_Keroro_Tag"));
	if (IA_TAG.Succeeded()) Tag = IA_TAG.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_MOUSE_RIGHT(TEXT("/Game/Input/IA_MOUSE_RIGHT_BUTTON.IA_MOUSE_RIGHT_BUTTON"));
	if (IA_MOUSE_RIGHT.Succeeded()) MouseRight = IA_MOUSE_RIGHT.Object;
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
		Input->BindAction(Running, ETriggerEvent::Triggered, this, &AKeroroPlayerController::StartRun);
		Input->BindAction(Running, ETriggerEvent::Completed, this, &AKeroroPlayerController::StopRun);
		Input->BindAction(Attacking, ETriggerEvent::Started, this, &AKeroroPlayerController::Attack);
		Input->BindAction(Tag, ETriggerEvent::Started, this, &AKeroroPlayerController::TagCharacter);
		Input->BindAction(MouseRight, ETriggerEvent::Started, this, &AKeroroPlayerController::OnMagicCircleActivated);
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


