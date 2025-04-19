// Fill out your copyright notice in the Description page of Project Settings.


#include "KeroroPlayerState.h"

AKeroroPlayerState::AKeroroPlayerState()
{
	CurrentKeroro = EKeroroType::Keroro;
	HP = 100.0f;
	MP = 50.0f;
	TotalScore = 0;
}

void AKeroroPlayerState::InitKeroroStatus(EKeroroType Type)
{
	CurrentKeroro = Type;

	switch (Type)
	{
	case EKeroroType::Keroro:
		HP = 100.0f;
		MP = 50.0f;
		break;
	case EKeroroType::Tamama:
		HP = 80.0f;
		MP = 80.0f;
		break;
	case EKeroroType::Giroro:
		HP = 120.0f;
		MP = 30.0f;
		break;
	case EKeroroType::Kururu:
		HP = 70.0f;
		MP = 100.0f;
		break;
	case EKeroroType::Dororo:
		HP = 90.0f;
		MP = 60.0f;
		break;
	default:
		HP = 100.0f;
		MP = 50.0f;
		break;
	}
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
