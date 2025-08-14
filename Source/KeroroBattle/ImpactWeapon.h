// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KeroroWeapon.h"
#include "ImpactWeapon.generated.h"

/**
 *
 */
UCLASS()
class KEROROBATTLE_API AImpactWeapon : public AKeroroWeapon
{
	GENERATED_BODY()

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	AImpactWeapon();
	virtual void PlayEffect(class AKeroroCharacter* Character) override;

	UFUNCTION()
	void AttackCheck_Impact();

private:
	float UltiDuration;
	FTimerHandle AttackTimer;
	FTimerHandle EffectTimer;

	TWeakObjectPtr<class AKeroroCharacter> OwnerKero;

private:
	UPROPERTY()
	UParticleSystem* ImpactPS;

	UPROPERTY()
	UParticleSystemComponent* ImpactPC;

	UPROPERTY()
	USoundWave* UltiHitSound;

};
