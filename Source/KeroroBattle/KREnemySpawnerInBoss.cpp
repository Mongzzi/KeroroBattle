// Fill out your copyright notice in the Description page of Project Settings.


#include "KREnemySpawnerInBoss.h"
#include "KeroroEnemyCharacter.h"
#include "Components/BoxComponent.h"
#include "KeroroGameInstance.h"
#include "KeroroGameState.h"

AKREnemySpawnerInBoss::AKREnemySpawnerInBoss() :AKREnemySpawner()
{

}

void AKREnemySpawnerInBoss::BeginPlay()
{
	AActor::BeginPlay();
	GS = GetWorld()->GetGameState<AKeroroGameState>();
	GI = GetGameInstance<UKeroroGameInstance>();
	if (!GI.IsValid() || !GS.IsValid())return;
	
	if (EnemyClass && GI->NextMissionRound == EKeroroType::Keroro)
	{
		GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AKREnemySpawner::SpawnEnemy, SpawnInterval, true);
	}
}

void AKREnemySpawnerInBoss::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
	Super::EndPlay(EndPlayReason);
}
