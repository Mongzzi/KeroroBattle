// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KeroroWeapon.h"
#include "RifleWeapon.generated.h"

/**
 * 
 */
UCLASS()
class KEROROBATTLE_API ARifleWeapon : public AKeroroWeapon
{
	GENERATED_BODY()
	ARifleWeapon();
	
	virtual void PlayEffect() override;
	virtual void PlaySound(int32 ComboIndex) override;
	virtual void Throw(const FVector& Direction, float Force) override;
	virtual void ReturnToHand(class AKeroroCharacter* Character) override;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* RifleFireSound;
};
