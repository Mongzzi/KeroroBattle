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

	virtual void PlayEffect(class AKeroroCharacter* Character) override;
	virtual void PlayHitSound(int32 CurrentCombo) override;
	virtual void PlaySound(int32 ComboIndex);

	UPROPERTY(EditAnywhere, Category = Sound)
	TArray<USoundWave*> AttackSound;

	UPROPERTY(EditAnywhere,Category= Sound)
	USoundBase* HitSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Effect)
	class UNiagaraSystem* NSEffect;

};
