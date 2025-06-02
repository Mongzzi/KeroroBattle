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
	virtual void PlaySound(int32 ComboIndex) override;

	void InitEffect(class AKeroroCharacter* kero);
	void ActivateMagicCircle();
	void ActivateFinalEffect();
	void AttackCheck_NoteBook();
	void DeactivateMagicCircle();
	void DeactivateMagicCircle2();
	void SpawnOrUpdateEffect(FVector Location, FRotator Rotation);
	void SpawnOrUpdateEffect_Particle(FVector Location, FRotator Rotation);
	void StopNoteBookAttack();

public:
	FVector FinalEffectLoc;

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
	UParticleSystem* MagicCircleEffect3;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	FTimerHandle DamageTickHandle;

	UPROPERTY()
	USoundBase* FinalEffectSound;

	bool bIsMagicCircleActive = false;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	float TraceDistance = 4000.f;

	UPROPERTY(EditDefaultsOnly,Category = "Effect")
	float EffectRemainTime = 3.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	float DamageTickInterval = 0.2f;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	float AttackRadius = 350.0f;

};
