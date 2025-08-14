// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KREnemySpawner.h"
#include "KREnemySpawnerInBoss.generated.h"

/**
 * 
 */
UCLASS()
class KEROROBATTLE_API AKREnemySpawnerInBoss : public AKREnemySpawner
{
	GENERATED_BODY()
public:
	AKREnemySpawnerInBoss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
