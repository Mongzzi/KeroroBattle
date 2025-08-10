// Fill out your copyright notice in the Description page of Project Settings.


#include "KeroroGameState.h"
#include "KeroroPlayerController.h"
#include "KeroroGameMode.h"

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
		if (RemainingTime < 0.0f&&!bIsTimeUp)
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
