// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ChatManager.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnChatUpdate,int32,ChatID ,FString, ChatText, TArray<FString>, SelectChatTexts, TArray<FString>, NextChatIDs);

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
};
