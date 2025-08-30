// Fill out your copyright notice in the Description page of Project Settings.


#include "ViperSpawner.h"
#include "KeroroEnemyCharacter.h"
#include "ViperCharacter.h"
#include "KeroroStatComponent.h"
#include "KeroroGameInstance.h"
#include "KeroroGameState.h"
#include "Kismet/GameplayStatics.h"

AViperSpawner::AViperSpawner() :AKREnemySpawner()
{
}

void AViperSpawner::BeginPlay()
{
	AActor::BeginPlay();
	GS = GetWorld()->GetGameState<AKeroroGameState>();
	GI = GetGameInstance<UKeroroGameInstance>();
	if (!GI.IsValid() || !GS.IsValid())return;

	if (GI->NextMissionRound != EKeroroType::Keroro)
	{
		SpawnInterval = 30.0f;
		GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AViperSpawner::SpawnEnemy, SpawnInterval, true);
	}
}

void AViperSpawner::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
	Super::EndPlay(EndPlayReason);
}

void AViperSpawner::SpawnEnemy()
{
	if (!GS.IsValid()) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AKeroroEnemyCharacter* Enemy = GetWorld()->SpawnActor<AKeroroEnemyCharacter>(AViperCharacter::StaticClass(), GetActorLocation(), GetActorRotation(), SpawnParams);

	if (Enemy->EnemyStat)
	{
		Enemy->EnemyStat->SetLevel(GS->EnemyLevel + 10);
		Enemy->EnemyStat->SetHP(Enemy->EnemyStat->MaxHp);
	}

	GS->Enemies.Add(Enemy);
}
