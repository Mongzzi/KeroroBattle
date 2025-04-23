// Fill out your copyright notice in the Description page of Project Settings.


#include "KeroroPlayerState.h"
#include "Kismet/GameplayStatics.h"

AKeroroPlayerState::AKeroroPlayerState()
{
	CurrentKeroro = EKeroroType::Keroro;
	TotalScore = 0;
}

void AKeroroPlayerState::BeginPlay()
{
	Super::BeginPlay();

}

EKeroroType AKeroroPlayerState::SetNextCharacterType()
{
	int32 NextType = static_cast<int32>(CurrentKeroro) + 1;

	if (NextType >= static_cast<int32>(EKeroroType::MAX))
	{
		 CurrentKeroro = EKeroroType::Keroro;
	}
	else
	{
		CurrentKeroro = static_cast<EKeroroType>(NextType);
	}
	return CurrentKeroro;
}
