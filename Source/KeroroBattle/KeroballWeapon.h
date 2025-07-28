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

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	AKeroballWeapon();

	virtual void Throw(const FVector& Direction, float Force) override;
	virtual void ReturnToHand(class AKeroroCharacter* Character) override;

	FTimerHandle ExplodeTimerHandle;

	void Explode();
	void FallDown();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,UPrimitiveComponent* OtherComp, FVector NormalImpulse,const FHitResult& Hit);

public:
	float BombTime = 1.25f;
	float BombSoundVolume = 3.0f;

	bool bIsThrowing;

private:
	UPROPERTY(EditAnywhere, Category = Sound)
	USoundBase* BombSound;

	UPROPERTY(EditDefaultsOnly, Category = Effect)
	class UNiagaraSystem* NSEffect;
};
