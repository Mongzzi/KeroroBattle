// Fill out your copyright notice in the Description page of Project Settings.


#include "KeroroCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "KeroroPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KeroroAnimInstance.h"

// Sets default values
AKeroroCharacter::AKeroroCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// �޽� ��ġ ����
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));

	// ī�޶� ��������
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 50.0f, 60.0f), FRotator(-15.0f, 0.0f, 0.0f));
	

	// �������� ����
	SpringArm->bUsePawnControlRotation = true; // ��Ʈ�ѷ� ���� ȸ��
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = true; // Yaw�� ���󰡰�
	SpringArm->bDoCollisionTest = true; // ���� ������ ���� �� ī�޶� �浹 ����

	// ī�޶� ����
	Camera->bUsePawnControlRotation = false;

	// ĳ���� ȸ�� ���� ����
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true; // �Է� ���� ���� �� ȸ��
	GetCharacterMovement()->bUseControllerDesiredRotation = false; // ��Ʈ�ѷ� ȸ�� ��Ȱ��ȭ

	// ĳ���� �ӵ�
	WalkSpeed = 600.0f;
	RunSpeed = 1200.0f;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	// �Է�
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC_KERORO(TEXT("/Game/Input/IMC_Keroro.IMC_Keroro"));
	if (IMC_KERORO.Succeeded())InputMappingContext = IMC_KERORO.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_MOVE(TEXT("/Game/Input/IA_Keroro_Move.IA_Keroro_Move"));
	if (IA_MOVE.Succeeded()) Moving = IA_MOVE.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_LOOK(TEXT("/Game/Input/IA_Keroro_Look.IA_Keroro_Look"));
	if (IA_MOVE.Succeeded()) Looking = IA_LOOK.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_JUMP(TEXT("/Game/Input/IA_Keroro_Jump.IA_Keroro_Jump"));
	if (IA_MOVE.Succeeded()) Jumping = IA_JUMP.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_RUN(TEXT("/Game/Input/IA_Keroro_Run.IA_Keroro_Run"));
	if (IA_RUN.Succeeded()) Running = IA_RUN.Object;

	// ���̷�Ż �޽�
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_KERORO(TEXT("/Game/Keroro_Model/keroro/keroro.keroro"));
	if (SK_KERORO.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_KERORO.Object);
	}

	// �ִϸ��̼�
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance>KERORO_ANIM(TEXT("/Game/Blueprints/Keroro_AnimInstance.Keroro_AnimInstance_C"));
	if (KERORO_ANIM.Succeeded())GetMesh()->SetAnimInstanceClass(KERORO_ANIM.Class);

}

// Called when the game starts or when spawned
void AKeroroCharacter::BeginPlay()
{
	Super::BeginPlay();

	KRPlayerContoller = Cast<AKeroroPlayerController>(GetController());
	if (KRPlayerContoller != nullptr)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(KRPlayerContoller->GetLocalPlayer());
		if (Subsystem != nullptr) {
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
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
	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(Moving, ETriggerEvent::Triggered, this, &AKeroroCharacter::Move);
		Input->BindAction(Looking, ETriggerEvent::Triggered, this, &AKeroroCharacter::Look);
		Input->BindAction(Jumping, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		Input->BindAction(Running, ETriggerEvent::Triggered, this, &AKeroroCharacter::StartRun);
		Input->BindAction(Running, ETriggerEvent::Completed, this, &AKeroroCharacter::StopRun);
	}
}

void AKeroroCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	KRAnim = Cast<UKeroroAnimInstance>(GetMesh()->GetAnimInstance());
	if (KRAnim != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("good 11111111111"));
	}

}

void AKeroroCharacter::Move(const FInputActionValue& Value)
{
	if (KRPlayerContoller)
	{
		// ũ��
		float FowardValue = Value.Get<FVector2D>().Y;
		float SideValue = Value.Get<FVector2D>().X;

		// ����
		const FRotator Rotation = KRPlayerContoller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		FVector FowardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		FVector SideDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(FowardDirection, FowardValue);
		AddMovementInput(SideDirection, SideValue);

		// �Ĺ��̵� üũ 
		if (FowardValue < -0.1f) KRAnim->bIsMovingBackward = true;
		else KRAnim->bIsMovingBackward = false;

	}
}

void AKeroroCharacter::Look(const FInputActionValue& Value)
{
	if (KRPlayerContoller)
	{
		AddControllerYawInput(Value.Get<FVector2D>().X);
		AddControllerPitchInput(Value.Get<FVector2D>().Y);
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

