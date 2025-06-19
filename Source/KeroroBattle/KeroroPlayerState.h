// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KeroroBattle.h"
#include "GameFramework/PlayerState.h"
#include "KeroroPlayerState.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnLevelChanged);
DECLARE_MULTICAST_DELEGATE(FOnGoldChanged);
DECLARE_MULTICAST_DELEGATE(FOnExpChanged);
DECLARE_MULTICAST_DELEGATE(FOnKillNumChanged)

/**
 *
 */
UCLASS()
class KEROROBATTLE_API AKeroroPlayerState : public APlayerState
{
	GENERATED_BODY()

	AKeroroPlayerState();
	virtual void PostInitializeComponents() override;


protected:
	virtual void BeginPlay() override;

public:
	FOnLevelChanged OnLevelChanged;
	FOnGoldChanged OnGoldChanged;
	FOnExpChanged OnExpChanged;
	FOnKillNumChanged OnKillNumChanged;

public:
	EKeroroType GetCurrentCharacterType() const { return CurrentKeroro; }
	void SetCurrentCharacterType(EKeroroType type) { CurrentKeroro = type; }
	EKeroroType SetNextCharacterType();

	float GetExpRatio() const;
	bool AddExp(int32 exp);
	void AddGold(int32 gold);
	void AddKillEnemyNum();
	void SetLevel(int32 lv);

private:
	struct FKRStatData* StatData;

protected:
	// 현재 케로로 캐릭터
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Keroro")
	EKeroroType CurrentKeroro;

	// 누적 점수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Keroro")
	int32 TotalScore;

public:
	// 플레이어 레벨
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Keroro")
	int32 CurrentLevel;

	// 경험치
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Keroro")
	int32 CurrentExp;

	// 골드
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Keroro")
	int32 CurrentGold;

	// 적 처치 수
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Keroro")
	int32 KilledEnemyNum;

	
public:
	// --------------------------------------- 아래로는 카드로 얻을 수 있는 스탯 (발사체 제외)------------------------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	float HealPowerRate_Enhanced;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	float HealPowerOnKill_Enhanced;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	float ExpGainRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	float GoldGainRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	float AttackSpeedRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	int32 ProjectileCount_Enhanced;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	float ProjectileScale_Enhanced;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	float InvincibilityTime_Enhanced;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	float CritDamageRate_Enhanced;

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
	float SkillCooldownRate_Enhanced;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	float EvasionRate_Enhanced;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KRStat)
	float DefenseRate_Enhanced;

};
