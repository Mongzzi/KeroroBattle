// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KeroroEnemyCharacter.h"
#include "ViperCharacter.generated.h"

/**
 * 
 */
UCLASS()
class KEROROBATTLE_API AViperCharacter : public AKeroroEnemyCharacter
{
	GENERATED_BODY()

public:
	AViperCharacter(const FObjectInitializer& ObjectInitializer);
	virtual void Attack() override;
	virtual void AttackCheck() override;
	
};
