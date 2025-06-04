// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KeroroWeapon.h"
#include "SwordWeapon.generated.h"

/**
 * 
 */
UCLASS()
class KEROROBATTLE_API ASwordWeapon : public AKeroroWeapon
{
	GENERATED_BODY()

public:
	ASwordWeapon();

	virtual void PlayEffect(class AKeroroCharacter* Character)override;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* BombSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Effect)
	class UNiagaraSystem* NSEffect;
};
