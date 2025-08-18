// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ChatManager.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChatUpdate, const FString&, ChatText, const TArray<FString>&, Selections);

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

    FOnChatUpdate OnChatUpdate;
public:

    UFUNCTION()
    void StartChat(int32 ChatID);

    UFUNCTION()
    void ShowNextChat();

    UFUNCTION()
    void SelectChatOption(int32 Index);

    UFUNCTION()
    void EndChat();
};
