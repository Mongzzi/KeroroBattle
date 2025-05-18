// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KeroroWeapon.h"
#include "KeroballWeapon.generated.h"

/**
 * 
 */
UCLASS()
class KEROROBATTLE_API AKeroballWeapon : public AKeroroWeapon
{
	GENERATED_BODY()
	AKeroballWeapon();

	virtual void Throw(const FVector& Direction, float Force) override;
	virtual void ReturnToHand(class AKeroroCharacter* Character) override;

};
