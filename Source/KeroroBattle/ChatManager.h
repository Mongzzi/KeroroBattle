// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ChatManager.generated.h"


/**
 * 
 */
UCLASS()
class KEROROBATTLE_API UChatManager : public UObject
{
	GENERATED_BODY()

public:
    int32 CurrentID;

    UPROPERTY()
    UDataTable* ChatDataTable;
    TWeakObjectPtr<class UKRChatWidget> ChatWidget;

public:

    UFUNCTION()
    void StartChat(int32 ChatID);
};
