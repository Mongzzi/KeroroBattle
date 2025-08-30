// Fill out your copyright notice in the Description page of Project Settings.


#include "KREnemySpawner.h"
#include "KeroroEnemyCharacter.h"
#include "RoboboCharacter.h"
#include "NunwawaCharacter.h"
#include "ViperCharacter.h"
#include "Components/BoxComponent.h"
#include "KeroroGameInstance.h"
#include "KeroroGameState.h"
#include "KeroroStatComponent.h"
#include "Kismet/GameplayStatics.h"

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

}

// Called when the game starts or when spawned
void AKREnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	GS = GetWorld()->GetGameState<AKeroroGameState>();
	GI = GetGameInstance<UKeroroGameInstance>();
	if (!GI.IsValid() || !GS.IsValid())return;

	if (GI->NextMissionRound != EKeroroType::Keroro)
	{
		GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AKREnemySpawner::SpawnEnemy, SpawnInterval, true);
	}

	FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);
	if (CurrentLevelName == TEXT("MainLevel1"))
	{
		EnemyClasses.Add(AKeroroEnemyCharacter::StaticClass());
	}
	else if (CurrentLevelName == TEXT("MainLevel2"))
	{
		EnemyClasses.Add(AKeroroEnemyCharacter::StaticClass());
		EnemyClasses.Add(ANunwawaCharacter::StaticClass());
	}
	else if (CurrentLevelName == TEXT("MainLevel3"))
	{
		EnemyClasses.Add(AKeroroEnemyCharacter::StaticClass());
		EnemyClasses.Add(ANunwawaCharacter::StaticClass());
		EnemyClasses.Add(ARoboboCharacter::StaticClass());
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



	int32 RandInt = FMath::RandRange(0, EnemyClasses.Num() - 1);

	AKeroroEnemyCharacter* Enemy = GetWorld()->SpawnActor<AKeroroEnemyCharacter>(EnemyClasses[RandInt], SpawnLocation, SpawnRotation, SpawnParams);
	if (Enemy && Enemy->EnemyStat)
	{
		if (Enemy->IsA(AViperCharacter::StaticClass()))
		{
			Enemy->EnemyStat->SetLevel(GS->EnemyLevel + 10);
		}
		else {
			Enemy->EnemyStat->SetLevel(GS->EnemyLevel);
		}
		Enemy->EnemyStat->SetHP(Enemy->EnemyStat->MaxHp);
	}
	GS->Enemies.Add(Enemy);
}

