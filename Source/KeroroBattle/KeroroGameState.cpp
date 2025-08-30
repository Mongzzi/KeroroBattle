// Fill out your copyright notice in the Description page of Project Settings.


#include "KeroroGameState.h"
#include "KeroroPlayerController.h"
#include "KeroroGameMode.h"
#include "KeroroEnemyCharacter.h"
#include "ViperCharacter.h"
#include "KeroroStatComponent.h"


AKeroroGameState::AKeroroGameState()
{
	PrimaryActorTick.bCanEverTick = true;
	RemainingTime = 30.0f;
	bIsTimeUp = false;
}

void AKeroroGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (RemainingTime > 0.0f)
	{
		RemainingTime -= DeltaSeconds;
		if (RemainingTime < 0.0f && !bIsTimeUp && bIsMainMap)
		{
			RemainingTime = 0.0f;
			bIsTimeUp = true;
			OnTimeOver();
		}
	}

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		AKeroroPlayerController* PC = Cast<AKeroroPlayerController>(It->Get());
		if (PC)
		{
			PC->UpdateTimeWidget(RemainingTime);
		}
	}

}

void AKeroroGameState::BeginPlay()
{
	Super::BeginPlay();
	bIsTimeUp = false;

	GetWorld()->GetTimerManager().SetTimer(EnemyLevelUpHandle, this, &AKeroroGameState::SetEnemyLevelFromTime, 20.0f, true);
}

void AKeroroGameState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetTimerManager().ClearTimer(EnemyLevelUpHandle);
	Super::EndPlay(EndPlayReason);
}



void AKeroroGameState::OnTimeOver()
{
	AGameModeBase* GM = GetWorld()->GetAuthGameMode();
	if (GM)
	{
		AKeroroGameMode* KeroGM = Cast<AKeroroGameMode>(GM);
		if (KeroGM)
		{
			KeroGM->OnTimeOver();
		}
	}
}

void AKeroroGameState::SetEnemyLevelFromTime()
{
	EnemyLevel = FMath::Clamp(EnemyLevel + 1, 1, 99);
	for (auto Enemy : Enemies)
	{
		if (IsValid(Enemy) && Enemy->EnemyStat)
		{
			if (Enemy->IsA(AViperCharacter::StaticClass()))
			{
				Enemy->EnemyStat->SetLevel(EnemyLevel + 10);
			}
			else {
				Enemy->EnemyStat->SetLevel(EnemyLevel);
			}
		}
	}
}
