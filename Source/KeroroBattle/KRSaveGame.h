// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KeroroBattle.h"
#include "GameFramework/SaveGame.h"
#include "KRSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class KEROROBATTLE_API UKRSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	bool bIsEntraceAnimPlayed = false;

	UPROPERTY()
	bool bIsManualChatPlayed = false;

	UPROPERTY()
	TMap<EKeroroType, bool> UnlockedChattings;

	UPROPERTY()
	TMap<EKeroroType, bool> UnlockedCharacters;
};
