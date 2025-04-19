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
    // ���� �ɷη� ĳ����
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Keroro")
    EKeroroType CurrentKeroro;

    // ü��
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Keroro")
    float HP;

    // ����
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Keroro")
    float MP;

    //���� ����
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Keroro")
    int32 TotalScore;

    // ĳ���� ���� �ʱ�ȭ
    UFUNCTION(BlueprintCallable, Category = "Keroro")
    void InitKeroroStatus(EKeroroType Type);

    //
    EKeroroType GetCurrentCharacterType() const { return CurrentKeroro; }
    void SetCurrentCharacterType(EKeroroType type) { CurrentKeroro = type; }
    EKeroroType SetNextCharacterType();

};
