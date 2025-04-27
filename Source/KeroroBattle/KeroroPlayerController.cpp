// Fill out your copyright notice in the Description page of Project Settings.


#include "KeroroPlayerController.h"
#include "KeroroCharacter.h"
#include "KeroroPlayerState.h"
#include "KeroroStatComponent.h"
#include "KeroroHUDWidget.h"
#include "KeroroGameState.h"
#include "KeroroAIController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

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
	//AKeroroCharacter* KRCharacter = Cast<AKeroroCharacter>(GetCharacter());
	//if (KRCharacter && KRHUDWidget)
	//{
	//	Cast<UKeroroHUDWidget>(KRHUDWidget)->BindKRStat(KRCharacter->KRStat);
	//}
}

void AKeroroPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AKeroroPlayerController::BeginPlay()
{
	Super::BeginPlay();
	KRPlayerState = Cast<AKeroroPlayerState>(PlayerState);
	AKeroroCharacter* KRCharacter = Cast<AKeroroCharacter>(GetCharacter());
	if (KRCharacter)
	{
		EKeroroType MyType = KRCharacter->GetKeroroCharacterType(); // 캐릭터가 자신의 타입 알려주는 함수
		CharacterMap.Add(MyType, KRCharacter); // TMap에 미리 등록
	}

	if (KRHUDWidgetClass)
	{
		KRHUDWidget = CreateWidget<UKeroroHUDWidget>(this, KRHUDWidgetClass);
		KRHUDWidget->BindKRStat(KRCharacter->KRStat);
		KRHUDWidget->AddToViewport();
	}

	if (KRHUDWidget)
	{
		if (KRCharacter)
		{
			KRCharacter->KRStat->OnHpIsChanged.AddUObject(this, &AKeroroPlayerController::UpdateHPWidget);
		}
	}
}

void AKeroroPlayerController::UpdateHPWidget()
{
	KRHUDWidget->UpdateHPWidget();
}

void AKeroroPlayerController::UpdateGoldWidget()
{
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
		UE_LOG(LogTemp, Warning, TEXT("spawn new character"));
		FVector SpawnLoc = GetCharacter()->GetActorLocation() + FVector(0, 0, 300);
		FRotator SpawnRot = GetCharacter()->GetActorRotation();
		NewCharacter = GetWorld()->SpawnActor<AKeroroCharacter>(AKeroroCharacter::StaticClass(), SpawnLoc, SpawnRot);
		if (NewCharacter)
		{
			NewCharacter->LoadAssetandSetting(NextType);
			CharacterMap.Add(NextType, NewCharacter);
		}
	}

	// 기존 캐릭터에 AIController 할당
	if (GetCharacter())
	{
		AKeroroCharacter* PreCharacter = Cast<AKeroroCharacter>(GetCharacter());
		AKeroroAIController* AIController = GetWorld()->SpawnActor<AKeroroAIController>(AKeroroAIController::StaticClass());

		if (AIController)
		{
			AIController->Possess(PreCharacter);  // AI 컨트롤러로 캐릭터 소유
		}
	}
	Possess(NewCharacter);


	// 캐릭터 스탯컴포넌트 hud에 바인딩, hp위젯 바뀐캐릭터로 초기화
	if (KRHUDWidget && NewCharacter)
	{
		KRHUDWidget->BindKRStat(NewCharacter->KRStat);
		UpdateHPWidget();
	}

	// 태그 이펙트
	if (NSTagEffect)
	{
		FVector EffectLoc = GetCharacter()->GetActorLocation() + GetCharacter()->GetActorForwardVector() * 100.0f;
		FRotator EffecRot = GetCharacter()->GetActorRotation();

		NCTagEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NSTagEffect, EffectLoc, EffecRot, FVector(1.0f));
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

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_RUN(TEXT("/Game/Input/IA_Keroro_Run.IA_Keroro_Run"));
	if (IA_RUN.Succeeded()) Running = IA_RUN.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_ATTACK(TEXT("/Game/Input/IA_Keroro_Attack.IA_Keroro_Attack"));
	if (IA_ATTACK.Succeeded()) Attacking = IA_ATTACK.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_TAG(TEXT("/Game/Input/IA_Keroro_Tag.IA_Keroro_Tag"));
	if (IA_TAG.Succeeded()) Tag = IA_TAG.Object;
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
	}
}



void AKeroroPlayerController::Move(const FInputActionValue& Value)
{
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
	AddYawInput(LookInput.X);
	AddPitchInput(LookInput.Y);
}

void AKeroroPlayerController::Jump()
{
	if (AKeroroCharacter* kero = Cast<AKeroroCharacter>(GetCharacter()))
	{
		kero->Jump();
	}
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
	if (AKeroroCharacter* kero = Cast<AKeroroCharacter>(GetCharacter()))
	{
		kero->Attack();

	}
}


