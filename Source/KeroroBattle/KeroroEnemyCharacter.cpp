// Fill out your copyright notice in the Description page of Project Settings.


#include "KeroroEnemyCharacter.h"
#include "KeroroStatComponent.h"
#include "KeroroAIController.h"
#include "KeroroAnimInstance.h"
#include "KeroroHPBarWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"

// Sets default values
AKeroroEnemyCharacter::AKeroroEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	EnemyStat = CreateDefaultSubobject<UKeroroStatComponent>(TEXT("KRSTAT"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SKMESH(TEXT("/Game/Keroro_Model/joriri/joriri.joriri"));
	if (SKMESH.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SKMESH.Object);
	}
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("KeroroCharacter"));
	GetCharacterMovement()->bOrientRotationToMovement = true;	// 이동 방향으로 자동 회전
	AIControllerClass = AKeroroAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;



	static ConstructorHelpers::FClassFinder<UAnimInstance>ANIM(TEXT("/Game/Blueprints/KR_AnimInstance.KR_AnimInstance_C"));
	if (ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(ANIM.Class);
	}

	// HP바 추가
	HPBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));
	HPBar->SetupAttachment(GetMesh());
	HPBar->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	HPBar->SetWidgetSpace(EWidgetSpace::Screen);
	HPBar->SetDrawSize(FVector2D(300.0f, 50.0f));
	static ConstructorHelpers::FClassFinder<UUserWidget>HUD(TEXT("/Game/Blueprints/KR_HPBar.KR_HPBar_C"));
	if (HUD.Succeeded()) HPBar->SetWidgetClass(HUD.Class);

}


// Called when the game starts or when spawned
void AKeroroEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	// HP바 위젯
	auto HpBarWidget = Cast<UKeroroHPBarWidget>(HPBar->GetUserWidgetObject());
	if (HpBarWidget != nullptr)
	{
		HpBarWidget->BindKRStat(EnemyStat);
	}
}

// Called every frame
void AKeroroEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKeroroEnemyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	EnemyAnim = Cast<UKeroroAnimInstance>(GetMesh()->GetAnimInstance());
	// 스탯컴포넌트 체력0 델리게이트 바인딩
	EnemyStat->OnHpIsZero.AddLambda([this]()->void {
		EnemyAnim->SetDeadAnim();
		SetActorEnableCollision(false);
		});
}

float AKeroroEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (bIsDead || EnemyStat == nullptr)
		return 0.0f;

	// 받은 데미지만큼 체력 감소
	EnemyStat->SetDamage(DamageAmount);

	// 체력이 0이하가 되면 die함수 호출
	if (EnemyStat->GetHpRatio() <= 0.0f)
	{
		Die();
	}

	return DamageAmount;
}

void AKeroroEnemyCharacter::Die()
{
	if (bIsDead) return;

	bIsDead = true;

	// AI 컨트롤러가 있으면 언포제스


	// 콜리전 끄기
	SetActorEnableCollision(false);

	// 일정 시간 후 소멸
	SetLifeSpan(5.0f);
}

