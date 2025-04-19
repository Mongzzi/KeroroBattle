// Fill out your copyright notice in the Description page of Project Settings.


#include "KeroroPlayerController.h"
#include "KeroroCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KeroroPlayerState.h"
#include "EngineUtils.h"

AKeroroPlayerController::AKeroroPlayerController()
{
	// ют╥б
	LoadInputActionAndMappingContext();
}

void AKeroroPlayerController::BeginPlay()
{
	Super::BeginPlay();
	KRPlayerState = Cast<AKeroroPlayerState>(PlayerState);
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

void AKeroroPlayerController::TagCharacter()
{
	UE_LOG(LogTemp, Warning, TEXT("ananananana"));
	auto NewCharacter = GetWorld()->SpawnActor<AKeroroCharacter>(AKeroroCharacter::StaticClass(), GetCharacter()->GetActorLocation()+FVector(0.0f,0.0f,300.0f), GetCharacter()->GetActorRotation());
	
	NewCharacter->LoadAssetandSetSkeletalMesh(KRPlayerState->SetNextCharacterType());
	Possess(NewCharacter);
}

