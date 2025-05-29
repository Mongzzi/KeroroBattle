// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KeroroWeapon.h"
#include "NoteBookWeapon.generated.h"

/**
 *
 */
UCLASS()
class KEROROBATTLE_API ANoteBookWeapon : public AKeroroWeapon
{
	GENERATED_BODY()

public:
	ANoteBookWeapon();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void InitEffect(class AKeroroCharacter* kero);
	void ActivateMagicCircle();
	void DeactivateMagicCircle();
	void SpawnOrUpdateEffect(FVector Location, FRotator Rotation);
	void SpawnOrUpdateEffect2(FVector Location, FRotator Rotation);

protected:
	UPROPERTY()
	class AKeroroCharacter* OwnerKero;

	UPROPERTY()
	class UNiagaraComponent* MagicCircleNiagara;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	class UNiagaraSystem* MagicCircleEffect;

	UPROPERTY()
	UParticleSystemComponent* MagicCircleParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	UParticleSystem* MagicCircleEffect2;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	float TraceDistance = 4000.f;

	bool bIsMagicCircleActive = false;
};
