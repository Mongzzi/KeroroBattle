// Fill out your copyright notice in the Description page of Project Settings.


#include "KeroroGameState.h"
#include "KeroroPlayerController.h"

AKeroroGameState::AKeroroGameState()
{
	PrimaryActorTick.bCanEverTick = true;
	RemainingTime = 30.0f;
}

void AKeroroGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (RemainingTime > 0.0f)
	{
		RemainingTime -= DeltaSeconds;
		if (RemainingTime < 0.0f)
		{
			RemainingTime = 0.0f;
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

}

void AKeroroGameState::OnTimeOver()
{
	UE_LOG(LogTemp, Warning, TEXT("Time Over!"));
}
