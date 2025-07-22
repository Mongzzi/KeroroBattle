// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KeroroWeapon.h"
#include "RotateWeapon.generated.h"

/**
 *
 */
UCLASS()
class KEROROBATTLE_API ARotateWeapon : public AKeroroWeapon
{
	GENERATED_BODY()

public:
	ARotateWeapon();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	TWeakObjectPtr<class AKeroroCharacter> OwnerKero;

public:
	void SetPositionWeapons();
	void CreateNSEffect();
	
	virtual void PlayEffect(class AKeroroCharacter* Character);

private:
	UFUNCTION()
	void OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY()
	TArray<USkeletalMeshComponent*> WeaponSKComponents;

	UPROPERTY(EditDefaultsOnly, Category = Effect)
	class UNiagaraSystem* NSEffect;


private:
	int32 WeaponNum;

	float SkillDuration;
	float RotateRadius;
	float RotationSpeed;
	float CurrentAngle;
};
