// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatManager.h"
#include "KeroroGameInstance.h"


void UChatManager::StartChat(int32 ChatID)
{

	if (!ChatDataTable) return;
	CurrentID = ChatID;

	FKRChatData* Row = ChatDataTable->FindRow<FKRChatData>(*FString::FromInt(CurrentID), TEXT(""));

	if (Row)
	{
		FString ChatText;
		if (!Row->ChatText.IsEmpty())
		{
			ChatText = Row->ChatText;
		}

		TArray<FString> SelectChatTexts;
		if (!Row->SelectChatTexts.IsEmpty())
		{
			Row->SelectChatTexts.ParseIntoArray(SelectChatTexts, TEXT("|"), true);
		}

		TArray<FString> NextIDs;
		if (!Row->NextChatIDs.IsEmpty())
		{
			Row->NextChatIDs.ParseIntoArray(NextIDs, TEXT("|"), true);
		}

		OnChatUpdate.Broadcast(CurrentID, Row->ChatText, SelectChatTexts, NextIDs);
	}
}

