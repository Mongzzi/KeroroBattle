// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KeroroBattle.h"
#include "GameFramework/PlayerState.h"
#include "KeroroPlayerState.generated.h"



/**
 *
 */
UCLASS()
class KEROROBATTLE_API AKeroroPlayerState : public APlayerState
{
	GENERATED_BODY()

	AKeroroPlayerState();

protected:
	virtual void BeginPlay() override;

protected:
	// 현재 케로로 캐릭터
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Keroro")
	EKeroroType CurrentKeroro;

	// 누적 점수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Keroro")
	int32 TotalScore;

	// 플레이어 레벨
	UPROPERTY(Transient)
	int32 KeroroLevel;

	// 경험치


	// 골드


	// 적 처치 수

public:
	EKeroroType GetCurrentCharacterType() const { return CurrentKeroro; }
	void SetCurrentCharacterType(EKeroroType type) { CurrentKeroro = type; }
	EKeroroType SetNextCharacterType();


};
