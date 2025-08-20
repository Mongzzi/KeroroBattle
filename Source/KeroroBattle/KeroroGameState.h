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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	float GetRemainingTime() { return RemainingTime; }

public:
	float RemainingTime;
	bool bIsTimeUp;
	bool bIsMainMap = false;
	int32 EnemyLevel = 1;

	FTimerHandle EnemyLevelUpHandle;

	UPROPERTY()
	TArray<class AKeroroEnemyCharacter*> Enemies;

public:
	void OnTimeOver();
	void SetEnemyLevelFromTime();

};
