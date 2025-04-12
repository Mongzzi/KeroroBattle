// Fill out your copyright notice in the Description page of Project Settings.


#include "KeroroCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"

// Sets default values
AKeroroCharacter::AKeroroCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));
	SpringArm->TargetArmLength = 200.0f;
	SpringArm->SetRelativeLocationAndRotation(FVector(0.0f,0.0f,20.0f), FRotator(-15.0f, 0.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_KERORO(TEXT("/Game/Keroro_Model/keroro/keroro.keroro"));
	if (SK_KERORO.Succeeded())GetMesh()->SetSkeletalMesh(SK_KERORO.Object);
	
	// ют╥б
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC_KERORO(TEXT("/Game/Input/IMC_Keroro.IMC_Keroro"));
	if (IMC_KERORO.Succeeded())InputMappingContext = IMC_KERORO.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_MOVE(TEXT("/Game/Input/IA_Keroro_Move.IA_Keroro_Move"));
	if (IA_MOVE.Succeeded()) Moving = IA_MOVE.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction>IA_LOOK(TEXT("/Game/Input/IA_Keroro_Look.IA_Keroro_Look"));
	if (IA_MOVE.Succeeded()) Looking = IA_LOOK.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction>IA_JUMP(TEXT("/Game/Input/IA_Keroro_Jump.IA_Keroro_Jump"));
	if (IA_MOVE.Succeeded())Jumping = IA_JUMP.Object;

}

// Called when the game starts or when spawned
void AKeroroCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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

void AKeroroCharacter::Move(const FInputActionValue& Value)
{
}

void AKeroroCharacter::Look(const FInputActionValue& Value)
{
}

