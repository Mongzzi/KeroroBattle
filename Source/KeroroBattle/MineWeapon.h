// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KeroroWeapon.h"
#include "MineWeapon.generated.h"

/**
 * 
 */
UCLASS()
class KEROROBATTLE_API AMineWeapon : public AKeroroWeapon
{
	GENERATED_BODY()
	
protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	AMineWeapon();

private: 
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditDefaultsOnly, Category = Effect)
	class UNiagaraSystem* NSEffect;

	UPROPERTY()
	USoundWave* UltiHitSound;

};
