// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KeroroBattle.h"
#include "GameFramework/PlayerState.h"
#include "KeroroPlayerState.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnLevelChanged);
DECLARE_MULTICAST_DELEGATE(FOnGoldChanged);
DECLARE_MULTICAST_DELEGATE(FOnExpChanged);

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
	FOnLevelChanged OnLevelChanged;
	FOnGoldChanged OnGoldChanged;
	FOnExpChanged OnExpChanged;

public:
	EKeroroType GetCurrentCharacterType() const { return CurrentKeroro; }
	void SetCurrentCharacterType(EKeroroType type) { CurrentKeroro = type; }
	EKeroroType SetNextCharacterType();

	bool AddExp(int32 exp);
	float GetExpRatio() const;
	
	void SetLevel(int32 lv);

private:
	struct FKRStatData* StatData;
};
