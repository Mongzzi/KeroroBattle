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

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Sound")
	TArray<USoundBase*> FistComboAttackSounds;		// 콤보에 따라 다른 사운드

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Effect)
	class UNiagaraSystem* NSEffect;
};
