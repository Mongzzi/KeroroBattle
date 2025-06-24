// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KeroroWeapon.h"
#include "FistWeapon.generated.h"

/**
 *
 */
UCLASS()
class KEROROBATTLE_API AFistWeapon : public AKeroroWeapon
{
	GENERATED_BODY()

public:
	AFistWeapon();
	void InitEffect(class AKeroroCharacter* kero);
	virtual void PlayHitSound(int32 ComboIndex) override;

protected:
	virtual void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditAnywhere,Category= Sound)
	TArray<USoundWave*> HitSounds;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Effect)
	class UNiagaraSystem* NSEffect;

	UPROPERTY(EditAnywhere, Category = Effect)
	TArray<class UNiagaraComponent*> NCEffects;
};
