// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KeroroBattle.h"
#include "Components/ActorComponent.h"
#include "KeroroStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);
DECLARE_MULTICAST_DELEGATE(FOnHPIsChangedDeleGate);

DECLARE_MULTICAST_DELEGATE(FOnMPIsChangedDeleGate);


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class KEROROBATTLE_API UKeroroStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UKeroroStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	void UpdateStatCardEnhanced(class AKeroroPlayerState* PlayerState);
	void SetLevel(int32 lv,class AKeroroPlayerState* PlayerState = nullptr);
	void SetDamage(float Damage);
	float SetFinalDamage(float Damage);
	void SetHP(float hp);
	void SetMP(float mp);
	float GetHpRatio();
	float GetMpRatio();
	int32 GetDropExp();
	int32 GetDropGold();

	FOnHPIsZeroDelegate OnHpIsZero;
	FOnHPIsChangedDeleGate OnHpIsChanged;
	FOnMPIsChangedDeleGate OnMpIsChanged;

private:
	FTimerHandle HealTimerHandle;
	float HealIntervalTime;
	
	FTimerHandle MpHealTimerHandle;
	float MpHealIntervalTime;

	FTimerHandle InvincibilityTimerHandle;
	bool bIsInvincible = false;

public:
	void StartAutoHeal();
	void StartAutoMpHeal();
	void AttackHeal();
	void AttackMpHeal();

	void StartInvincibility();
	void EndInvincibility();

	bool PlayUltiSkill();

private:
	struct FKRStatData* StatData;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	float CurrentHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	float CurrentMp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	float UltiCostMp;

	// --------------------------레벨에 따라 스탯 데이터 테이블로 부터 받아와야하는 정보 ----------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	float MaxMp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	float AttackPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	int32 DropExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	int32 NextExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	float MaxMoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	float CritChanceRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	float SkillCooldownRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	float EvasionRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	float DefenseRate;

	// --------------------------------------- 아래로는 카드로 얻은 추가 스탯 ------------------------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	float HealPowerRate;
	float HealPowerRate_Default;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	float HealPowerOnKill;
	float HealPowerOnKill_Default;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	int32 ProjectileCount;
	int32 ProjectileCount_Default;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	float ProjectileScale;
	float ProjectileScale_Default;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	float InvincibilityTime;
	float InvincibilityTime_Default;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	float CritDamageRate;
	float CritDamageRate_Default;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	float GuardTime;
	float GuardTime_Default;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	float ParryTime;
	float ParryTime_Default;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	float GuardCoolTime;
	float GuardCollTime_Default;

};
