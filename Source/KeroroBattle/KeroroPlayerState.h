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

public:
    // 현재 케로로 캐릭터
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Keroro")
    EKeroroType CurrentKeroro;

    //누적 점수
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Keroro")
    int32 TotalScore;

    //
    EKeroroType GetCurrentCharacterType() const { return CurrentKeroro; }
    void SetCurrentCharacterType(EKeroroType type) { CurrentKeroro = type; }
    EKeroroType SetNextCharacterType();


};
