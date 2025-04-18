// Fill out your copyright notice in the Description page of Project Settings.


#include "KeroroCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "KeroroPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KeroroAnimInstance.h"
#include "KeroroWeapon.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

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

	// ĸ��������Ʈ �ݸ����������� ���� 
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("KeroroCharacter"));

	// ���̾ư��� ����Ʈ �߰�
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NE(TEXT("/Game/Knife_light/VFX/NE_attack02.NE_attack02"));
	if (NE.Succeeded())
	{
		NSAttackEffect = NE.Object;
	}

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

	// ���� üũ - ���� �����ʿ� ��ӹ��� Ŭ������ �־���� // ���⸶�� �ٸ���
	IsAttacking = false;
	MaxCombo = 4;
	AttackEndComboState();

	// ���̷�Ż �޽�
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_KERORO(TEXT("/Game/Keroro_Model/kururu/kururu.kururu"));
	if (SK_KERORO.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_KERORO.Object);
	}

	// �ִϸ��̼�
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance>KERORO_ANIM(TEXT("/Game/Blueprints/KR_AnimInstance.KR_AnimInstance_C"));
	if (KERORO_ANIM.Succeeded())GetMesh()->SetAnimInstanceClass(KERORO_ANIM.Class);

}

// Called when the game starts or when spawned
void AKeroroCharacter::BeginPlay()
{
	Super::BeginPlay();

	FName WeaponSocket(TEXT("hand_rSocket"));
	auto CurWeapon = GetWorld()->SpawnActor<AKeroroWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (CurWeapon)
	{
		// ������ ���� ( ���⸶�� �ڿ������� ������ - ���� ���������̺� ���� �� ������ ����)
		CurWeapon->SetActorRelativeLocation(CurWeapon->AttachLocationOffset);
		CurWeapon->SetActorRelativeRotation(CurWeapon->AttachRotationOffset);
		CurWeapon->SetActorRelativeScale3D(CurWeapon->AttachScale);

		CurWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
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
	//if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	//{
	//	Input->BindAction(Moving, ETriggerEvent::Triggered, this, &AKeroroCharacter::Move);
	//	Input->BindAction(Looking, ETriggerEvent::Triggered, this, &AKeroroCharacter::Look);
	//	Input->BindAction(Jumping, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	//	Input->BindAction(Running, ETriggerEvent::Triggered, this, &AKeroroCharacter::StartRun);
	//	Input->BindAction(Running, ETriggerEvent::Completed, this, &AKeroroCharacter::StopRun);
	//	Input->BindAction(Attacking, ETriggerEvent::Started, this, &AKeroroCharacter::Attack); // Ʈ�����̺�Ʈ - Started Ŭ�� �ѹ��� ���ε��� �Լ� �ѹ���ȣ��
	//	Input->BindAction(Tag, ETriggerEvent::Started, this, &AKeroroCharacter::TagCharacter);
	//}
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
	KRAnim->OnEffectCreateCheck.AddUObject(this, &AKeroroCharacter::PlaySwordEffect);
}

void AKeroroCharacter::Attack()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack succed"));

	if (IsAttacking) // �ִϸ��̼�(��Ÿ��) ������ΰ�
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

void AKeroroCharacter::PlaySwordEffect()
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		NSAttackEffect,
		GetActorLocation() + GetActorForwardVector() * 100.0f, // ĳ���� �� �������� 100 ���� �̵�
		GetActorRotation(),
		FVector(2.0f)
	);
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

void AKeroroCharacter::TagCharacter()
{
	//if (!KRPlayerContoller) return;

	//// ���� ���忡 �����ϴ� KeroroCharacter ĳ���͵� ��������
	//TArray<AKeroroCharacter*> FoundCharacters;
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), AKeroroCharacter::StaticClass(), FoundCharacters);

	//if (FoundCharacters.Num() <= 1) return;

	//// ���� ��Ʈ�� ���� ĳ���� �ε��� ã��
	//int32 CurrentIndex = FoundCharacters.IndexOfByPredicate([this](AActor* Actor)
	//	{
	//		return Actor == this;
	//	});

	//// ���� ĳ���� �ε����� �̵� (�ݺ��ǵ���)
	//int32 NextIndex = (CurrentIndex + 1) % FoundCharacters.Num();
	//ACharacter* NextCharacter = Cast<ACharacter>(FoundCharacters[NextIndex]);

	//if (NextCharacter && KRPlayerContoller)
	//{
	//	// ���� ĳ���Ϳ��� ��Ʈ�� �����ϰ� ���� ĳ���ͷ� ��Ʈ�� �̵�
	//	KRPlayerContoller->UnPossess();
	//	KRPlayerContoller->Possess(NextCharacter);
	//}
}

void AKeroroCharacter::OnMeshLoaded()
{
	//UE_LOG(LogTemp, Warning, TEXT("Mesh load complete!"));

	//USkeletalMesh* AssetLoaded = Cast<USkeletalMesh>(AssetStreamingHandle->GetLoadedAsset());
	//AssetStreamingHandle.Reset();
	//if (AssetLoaded != nullptr){
	//	GetMesh()->SetSkeletalMesh(AssetLoaded);
	//}
}