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

public:
    // 현재 케로로 캐릭터
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Keroro")
    EKeroroType CurrentKeroro;

    // 체력
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Keroro")
    float HP;

    // 마나
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Keroro")
    float MP;

    //누적 점수
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Keroro")
    int32 TotalScore;

    // 캐릭터 스탯 초기화
    UFUNCTION(BlueprintCallable, Category = "Keroro")
    void InitKeroroStatus(EKeroroType Type);

    //
    EKeroroType GetCurrentCharacterType() const { return CurrentKeroro; }
    void SetCurrentCharacterType(EKeroroType type) { CurrentKeroro = type; }
    EKeroroType SetNextCharacterType();

};
