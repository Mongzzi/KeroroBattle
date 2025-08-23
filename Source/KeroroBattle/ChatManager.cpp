// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatManager.h"
#include "KRChatWidget.h"
#include "KeroroGameInstance.h"


void UChatManager::StartChat(int32 ChatID)
{
	if (!ChatDataTable || !ChatWidget.IsValid()) return;
	CurrentID = ChatID;

	FKRChatData* Row = ChatDataTable->FindRow<FKRChatData>(*FString::FromInt(CurrentID), TEXT(""));

	if (Row)
	{
		int32 ImageID = Row->ImageID;

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

		if (ChatWidget.IsValid())
		{
			ChatWidget->UpdateChatInfo(CurrentID, ImageID, Row->ChatText, SelectChatTexts, NextIDs);
		}
	}
}

