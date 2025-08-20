// Fill out your copyright notice in the Description page of Project Settings.


#include "KREnemySpawner.h"
#include "KeroroEnemyCharacter.h"
#include "Components/BoxComponent.h"
#include "KeroroGameInstance.h"
#include "KeroroGameState.h"
#include "KeroroStatComponent.h"

// Sets default values
AKREnemySpawner::AKREnemySpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnBox"));
	RootComponent = SpawnBox;
	SpawnBox->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));

	SpawnBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SpawnBox->SetHiddenInGame(true);

	EnemyClass = AKeroroEnemyCharacter::StaticClass();
}

// Called when the game starts or when spawned
void AKREnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	GS = GetWorld()->GetGameState<AKeroroGameState>();
	GI = GetGameInstance<UKeroroGameInstance>();
	if (!GI.IsValid() || !GS.IsValid())return;

	if (EnemyClass && GI->NextMissionRound != EKeroroType::Keroro)
	{
		GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AKREnemySpawner::SpawnEnemy, SpawnInterval, true);
	}
}

// Called every frame
void AKREnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKREnemySpawner::SpawnEnemy()
{
	if (!GS.IsValid()) return;
	EnemyNum = GS->Enemies.Num();

	if (EnemyNum >= MaxEnemyCount) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRotation = GetActorRotation();

	AKeroroEnemyCharacter* Enemy = GetWorld()->SpawnActor<AKeroroEnemyCharacter>(EnemyClass, SpawnLocation, SpawnRotation, SpawnParams);
	if (Enemy->EnemyStat)
	{
		Enemy->EnemyStat->SetLevel(GS->EnemyLevel);
	}

	GS->Enemies.Add(Enemy);
}

