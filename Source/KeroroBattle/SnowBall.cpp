// Fill out your copyright notice in the Description page of Project Settings.


#include "SnowBall.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "KeroroCharacter.h"
#include "KeroroEnemyCharacter.h"
#include "KeroroStatComponent.h"
#include "Engine/DamageEvents.h"
#include "CriticalDamageType.h"


// Sets default values
ASnowBall::ASnowBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SnowballMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SnowballMesh"));
	SnowballMesh->SetCollisionProfileName(TEXT("EnemyThrowingObj"));
	SnowballMesh->SetGenerateOverlapEvents(true);
	SnowballMesh->OnComponentBeginOverlap.AddDynamic(this, &ASnowBall::OnBeginOverlap);

	RootComponent = SnowballMesh;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("/Game/HandPaintedEnvironment/Assets/Models/Woodmill_Saw.Woodmill_Saw"));
	if (MeshAsset.Succeeded())
	{
		SnowballMesh->SetStaticMesh(MeshAsset.Object);
		SnowballMesh->SetRelativeScale3D(FVector(0.3f));
	}

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.0f;

	InitialLifeSpan = 5.0f;
}

// Called when the game starts or when spawned
void ASnowBall::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASnowBall::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ASnowBall::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && Cast<AKeroroCharacter>(OtherActor))
	{
		AKeroroEnemyCharacter* Enemykero = Cast<AKeroroEnemyCharacter>(GetInstigator());
		if (Enemykero == nullptr) return;

		UKeroroStatComponent* EnemyStat = Enemykero->EnemyStat;
		if (EnemyStat == nullptr) return;
		
		float FinalDamage = EnemyStat->AttackPower;
		bool bIsCritical = (FMath::FRand() < EnemyStat->CritChanceRate);

		FDamageEvent DamageEvent;
		if (bIsCritical)
		{
			FinalDamage *= EnemyStat->CritDamageRate;
			DamageEvent.DamageTypeClass = UCriticalDamageType::StaticClass();
		}

		OtherActor->TakeDamage(FinalDamage, DamageEvent, GetInstigatorController(), GetInstigator());
	}
	Destroy();
}


