// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KeroroStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);
DECLARE_MULTICAST_DELEGATE(FOnHPIsChangedDeleGate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
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

public:
	void SetLevel(int32 lv);
	void SetDamage(float dm);
	void SetHP(float hp);
	float GetHpRatio();
	int32 GetDropExp();

	FOnHPIsZeroDelegate OnHpIsZero;
	FOnHPIsChangedDeleGate OnHpIsChanged;

public:
	//void UpdateEnhanced();

private:
	struct FKRStatData* StatData;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	float CurrentHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	float CurrentMP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	float MaxMP;

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
	float AttackSpeedRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	float SkillCooldownRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	float EvasionRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	float DefenseRate;

	// --------------------------------------- 아래로는 카드로 얻은 추가 스탯 ------------------------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	float HealPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	float HealPowerOnKill;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	float ExpGainRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	float GoldGainRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	int32 ProjectileCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	float ProjectileScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	float InvincibilityTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	float CritDamage;

	//--------------------------------------- 스탯데이터 + a 항목------------------------------------------------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	float MaxHP_Enhanced;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	float MaxMP_Enhanced;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	float MaxMoveSpeed_Enhanced;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	float CritChanceRate_Enhanced;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	float AttackSpeedRate_Enhanced;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	float SkillCooldownRate_Enhanced;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	float EvasionRate_Enhanced;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	float DefenseRate_Enhanced;

};
