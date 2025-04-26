// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KeroroBattle.h"
#include "GameFramework/GameStateBase.h"
#include "KeroroGameState.generated.h"

/**
 * 
 */
UCLASS()
class KEROROBATTLE_API AKeroroGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AKeroroGameState();

	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

	float GetRemainingTime() { return RemainingTime; }
	

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GameRules)
	float TotalTime = 300.0f;
	float RemainingTime;
	void OnTimeOver();
};
