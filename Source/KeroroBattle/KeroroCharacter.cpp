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

	// 메시 위치 조정
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));

	// 카메라 스프링암
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 50.0f, 60.0f), FRotator(-15.0f, 0.0f, 0.0f));

	// 캡슐컴포넌트 콜리전프로파일 설정 
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("KeroroCharacter"));

	// 나이아가라 이펙트 추가
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NE(TEXT("/Game/Knife_light/VFX/NE_attack02.NE_attack02"));
	if (NE.Succeeded())
	{
		NSAttackEffect = NE.Object;
	}

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
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_KERORO(TEXT("/Game/Keroro_Model/kururu/kururu.kururu"));
	if (SK_KERORO.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_KERORO.Object);
	}

	// 애니메이션
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
		// 오프셋 적용 ( 무기마다 자연스러운 포지션 - 추후 데이터테이블 생성 후 꺼내올 예정)
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
	//	Input->BindAction(Attacking, ETriggerEvent::Started, this, &AKeroroCharacter::Attack); // 트리거이벤트 - Started 클릭 한번에 바인딩된 함수 한번만호출
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

void AKeroroCharacter::PlaySwordEffect()
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		NSAttackEffect,
		GetActorLocation() + GetActorForwardVector() * 100.0f, // 캐릭터 앞 방향으로 100 유닛 이동
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

	//// 현재 월드에 존재하는 KeroroCharacter 캐릭터들 가져오기
	//TArray<AKeroroCharacter*> FoundCharacters;
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), AKeroroCharacter::StaticClass(), FoundCharacters);

	//if (FoundCharacters.Num() <= 1) return;

	//// 현재 컨트롤 중인 캐릭터 인덱스 찾기
	//int32 CurrentIndex = FoundCharacters.IndexOfByPredicate([this](AActor* Actor)
	//	{
	//		return Actor == this;
	//	});

	//// 다음 캐릭터 인덱스로 이동 (반복되도록)
	//int32 NextIndex = (CurrentIndex + 1) % FoundCharacters.Num();
	//ACharacter* NextCharacter = Cast<ACharacter>(FoundCharacters[NextIndex]);

	//if (NextCharacter && KRPlayerContoller)
	//{
	//	// 현재 캐릭터에서 컨트롤 해제하고 다음 캐릭터로 컨트롤 이동
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