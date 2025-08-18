// Fill out your copyright notice in the Description page of Project Settings.


#include "KREnemySpawnerInBoss.h"
#include "KeroroEnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "KeroroGameInstance.h"
AKREnemySpawnerInBoss::AKREnemySpawnerInBoss():AKREnemySpawner()
{

}

void AKREnemySpawnerInBoss::BeginPlay()
{
	AActor::BeginPlay();
	UKeroroGameInstance* GI = GetGameInstance<UKeroroGameInstance>();
	if (EnemyClass && GI && GI->NextMissionRound == EKeroroType::Keroro)
	{
		GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AKREnemySpawner::SpawnEnemy, SpawnInterval, true);
	}
}

void AKREnemySpawnerInBoss::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
	Super::EndPlay(EndPlayReason);
}
