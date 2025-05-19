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

public:
	AKeroballWeapon();

	virtual void Throw(const FVector& Direction, float Force) override;
	virtual void ReturnToHand(class AKeroroCharacter* Character) override;

	FTimerHandle ExplodeTimerHandle;

	void Explode();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,UPrimitiveComponent* OtherComp, FVector NormalImpulse,const FHitResult& Hit);


	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* BombSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Effect)
	class UNiagaraSystem* NSEffect;
};
