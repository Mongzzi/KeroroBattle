// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KeroroBattle.h"
#include "GameFramework/GameModeBase.h"
#include "KeroroGameMode.generated.h"

/**
 * 
 */
UCLASS()
class KEROROBATTLE_API AKeroroGameMode : public AGameModeBase
{
	GENERATED_BODY()

	AKeroroGameMode();
	
protected:
	virtual void BeginPlay() override;

public:
	void OnTimeOver();
	void EndGame(bool bIsVictory);

private:
	UPROPERTY()
	class AKeroroGameState* KeroroGameState;

private:
	UPROPERTY()
	TSubclassOf<class UKRMissionEndWidget> MissionWidgetClass;

	class UKRMissionEndWidget* MissionEndWidget;

	float SurvivalTime = 180.0f;
};
