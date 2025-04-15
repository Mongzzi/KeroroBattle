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

	// 메시 위치 조정
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));

	// 카메라 스프링암
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 50.0f, 60.0f), FRotator(-15.0f, 0.0f, 0.0f));


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

	// 스켈레탈 메쉬
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_KERORO(TEXT("/Game/Keroro_Model/keroro/keroro.keroro"));
	if (SK_KERORO.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_KERORO.Object);
	}

	// 애니메이션
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance>KERORO_ANIM(TEXT("/Game/Blueprints/Keroro_AnimInstance.Keroro_AnimInstance_C"));
	if (KERORO_ANIM.Succeeded())GetMesh()->SetAnimInstanceClass(KERORO_ANIM.Class);

	// 소켓
	FName WeaponSocket(TEXT("kkkk"));	// 케로로 스켈레탈메쉬에 소켓 직접 추가함 다른캐릭도 같은 이름으로 필요
	if (GetMesh()->DoesSocketExist(WeaponSocket))
	{
		Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
		static ConstructorHelpers::FObjectFinder<USkeletalMesh>WEAPON(TEXT("/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_BlackKnight/SK_Blade_BlackKnight.SK_Blade_BlackKnight"));
		if (WEAPON.Succeeded())
		{
			Weapon->SetRelativeScale3D(FVector(10.0f, 10.0f, 10.0f));
			Weapon->SetSkeletalMesh(WEAPON.Object);
		}
		UE_LOG(LogTemp, Warning, TEXT("ok weapon"));
		Weapon->SetupAttachment(GetMesh(), WeaponSocket);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("failed weapon"));
	}

	// 입력
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
		Input->BindAction(Attacking, ETriggerEvent::Started, this, &AKeroroCharacter::Attack); // 트리거이벤트 - Started 클릭 한번에 바인딩된 함수 한번만호출
	}
}

void AKeroroCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	KRAnim = Cast<UKeroroAnimInstance>(GetMesh()->GetAnimInstance());
	if (KRAnim == nullptr) return;

	KRAnim->OnMontageEnded.AddDynamic(this, &AKeroroCharacter::OnAttackMontageEnded);
	KRAnim->OnNextAttackCheck.AddLambda([this]()->void {
		CanNextCombo = false;
		if (IsComboInputOn)
		{
			AttackStartComboState();
			KRAnim->JumptoAttackMontageSection(CurrentCombo);
			IsAttacking = true;
		}
		});
}

void AKeroroCharacter::Attack()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack succed"));

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

void AKeroroCharacter::Move(const FInputActionValue& Value)
{
	if (KRPlayerContoller && !IsAttacking)
	{
		// 크기
		float FowardValue = Value.Get<FVector2D>().Y;
		float SideValue = Value.Get<FVector2D>().X;

		// 방향
		const FRotator Rotation = KRPlayerContoller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		FVector FowardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		FVector SideDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(FowardDirection, FowardValue);
		AddMovementInput(SideDirection, SideValue);

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

