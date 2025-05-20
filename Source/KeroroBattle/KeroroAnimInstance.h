// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KeroroBattle.h"
#include "Animation/AnimInstance.h"
#include "KeroroAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnEffectCreateCheckDelegate);

/**
 *
 */
UCLASS()
class KEROROBATTLE_API UKeroroAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UKeroroAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayAttackMontage();
	void JumptoAttackMontageSection(int32 NewSection);

public:
	FOnNextAttackCheckDelegate OnNextAttackCheck;
	FOnAttackHitCheckDelegate OnAttackHitCheck;
	FOnEffectCreateCheckDelegate OnEffectCreateCheck;
	
	void SetDeadAnim();
	void SetWeaponType(EWeaponType type);
	UAnimMontage* GetWeaponMontage();

private:
	UFUNCTION()
	void AnimNotify_AttackHitCheck();

	UFUNCTION()
	void AnimNotify_NextAttackCheck();

	UFUNCTION()
	void AnimNotify_EffectCreateCheck();

	FName GetAttackMontageSectionName(int32 Section);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn)
	float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn)
	bool IsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn)
	bool bIsRunning;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	EWeaponType WeaponType;

private:
	UPROPERTY(EditAnywhere, Category = "Attack")
	TMap<EWeaponType, UAnimMontage*> AttackMontages;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	class UAnimMontage* SwordAttackMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category= Attack, Meta=(AllowPrivateAccess=true))
	class UAnimMontage* RifleAttackMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	class UAnimMontage* KeroBallAttackMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	class UAnimMontage* FistAttackMontage;


	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Pawn,meta=(AllowPrivateAccess=true))
	bool IsDead;
};

