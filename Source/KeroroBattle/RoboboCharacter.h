// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KeroroEnemyCharacter.h"
#include "RoboboCharacter.generated.h"

/**
 * 
 */
UCLASS()
class KEROROBATTLE_API ARoboboCharacter : public AKeroroEnemyCharacter
{
	GENERATED_BODY()
	

public:
	ARoboboCharacter(const FObjectInitializer& ObjectInitializer);
	virtual void Attack() override;
	virtual void AttackCheck() override;

private:
	UPROPERTY()
	class UParticleSystemComponent* ParticleComponent;

	UPROPERTY()
	class UParticleSystem* ParticleSystem;

};
