// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KREnemySpawner.h"
#include "ViperSpawner.generated.h"

/**
 * 
 */
UCLASS()
class KEROROBATTLE_API AViperSpawner : public AKREnemySpawner
{
	GENERATED_BODY()
	
public:
	AViperSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void SpawnEnemy() override;

};
