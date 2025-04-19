// Fill out your copyright notice in the Description page of Project Settings.


#include "KeroroCharacter.h"
#include "KeroroPlayerController.h"
#include "KeroroAnimInstance.h"
#include "KeroroWeapon.h"
#include "KeroroPlayerState.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
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
}

// Called when the game starts or when spawned
void AKeroroCharacter::BeginPlay()
{
	Super::BeginPlay();

	// (���̷�Ż�޽�,�ִ��ν��Ͻ� �ε� �� ����),(��Ÿ�� ��������Ʈ ���ε�)
	LoadAssetandSetting(CurrentKeroroType);

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
}

void AKeroroCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//BindAnimInstanceEvents();
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

void AKeroroCharacter::BindAnimInstanceEvents()
{	

	KRAnim = Cast<UKeroroAnimInstance>(GetMesh()->GetAnimInstance());
	if (KRAnim == nullptr) return;

	// ��Ÿ�� ������ �� �����޺� �ʱ�ȭ
	KRAnim->OnMontageEnded.AddDynamic(this, &AKeroroCharacter::OnAttackMontageEnded);
	
	// ���� ����
	KRAnim->OnNextAttackCheck.AddLambda([this]()->void {
		CanNextCombo = false;
		if (IsComboInputOn)
		{
			AttackStartComboState();
			KRAnim->JumptoAttackMontageSection(CurrentCombo);
			IsAttacking = true;
		}
		});

	// ���� ����Ʈ
	KRAnim->OnEffectCreateCheck.AddUObject(this, &AKeroroCharacter::PlaySwordEffect);
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

void AKeroroCharacter::LoadAssetandSetting(EKeroroType type)
{
	USkeletalMesh* NewMesh = nullptr;

	switch (type)
	{
	case EKeroroType::Keroro:
		NewMesh = LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Keroro_Model/keroro/keroro.keroro"));
		break;
	case EKeroroType::Tamama:
		NewMesh = LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Keroro_Model/tamama/tamama.tamama"));
		break;
	case EKeroroType::Giroro:
		NewMesh = LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Keroro_Model/giroro/giroro.giroro"));
		break;
	case EKeroroType::Kururu:
		NewMesh = LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Keroro_Model/kururu/kururu.kururu"));
		break;
	case EKeroroType::Dororo:
		NewMesh = LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Keroro_Model/dororo/dororo.dororo"));
		break;
	}

	if (NewMesh)
	{
		GetMesh()->SetSkeletalMesh(NewMesh);

		// �ִ� �ν��Ͻ� ����
		UClass* AnimBPClass = LoadClass<UAnimInstance>(nullptr, TEXT("/Game/Blueprints/KR_AnimInstance.KR_AnimInstance_C"));
		if (AnimBPClass)
		{
			GetMesh()->SetAnimInstanceClass(AnimBPClass);
		}

		// �ִ��ν��Ͻ� ���� �� ��������Ʈ ���ε�
		BindAnimInstanceEvents();
	}
}