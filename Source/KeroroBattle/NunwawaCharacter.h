// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KeroroEnemyCharacter.h"
#include "NunwawaCharacter.generated.h"

/**
 * 
 */
UCLASS()
class KEROROBATTLE_API ANunwawaCharacter : public AKeroroEnemyCharacter
{
	GENERATED_BODY()
	
public:
	ANunwawaCharacter(const FObjectInitializer& ObjectInitializer);
	virtual void Attack() override;
	virtual void AttackCheck() override;
};
