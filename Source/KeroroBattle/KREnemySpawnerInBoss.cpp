// Fill out your copyright notice in the Description page of Project Settings.


#include "KREnemySpawnerInBoss.h"
#include "KeroroEnemyCharacter.h"
#include "RoboboCharacter.h"
#include "NunwawaCharacter.h"
#include "ViperCharacter.h"
#include "Components/BoxComponent.h"
#include "KeroroGameInstance.h"
#include "KeroroGameState.h"
#include "Kismet/GameplayStatics.h"

AKREnemySpawnerInBoss::AKREnemySpawnerInBoss() :AKREnemySpawner()
{

}

void AKREnemySpawnerInBoss::BeginPlay()
{
	AActor::BeginPlay();
	GS = GetWorld()->GetGameState<AKeroroGameState>();
	GI = GetGameInstance<UKeroroGameInstance>();
	if (!GI.IsValid() || !GS.IsValid())return;

	if (GI->NextMissionRound == EKeroroType::Keroro)
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
		if (GI->NextMissionRound == EKeroroType::Keroro) {
			EnemyClasses.Add(AViperCharacter::StaticClass());
		}
	}
}

void AKREnemySpawnerInBoss::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
	Super::EndPlay(EndPlayReason);
}
