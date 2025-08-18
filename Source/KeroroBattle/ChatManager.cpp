// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatManager.h"
#include "KeroroGameInstance.h"


void UChatManager::StartChat(int32 ChatID)
{
    CurrentID = ChatID;
    ShowNextChat();
}

void UChatManager::ShowNextChat()
{
    if (!ChatDataTable) return;

    FKRChatData* Row = ChatDataTable->FindRow<FKRChatData>(*FString::FromInt(CurrentID),TEXT(""));

    if (Row)
    {
        // 선택지 파싱 |전후로 나눠서 스트링에넣어줌
        TArray<FString> SelectChatTexts;
        if (!Row->SelectChatTexts.IsEmpty())
        {
            Row->SelectChatTexts.ParseIntoArray(SelectChatTexts, TEXT("|"), true);
        }

        // 대화 , 선택지 내용 델리게이트로 넘겨줌 추후 위젯클래스에서 바인딩할 함수 추가해야함
        OnChatUpdate.Broadcast(Row->ChatText, SelectChatTexts);

        // 선택지가 없으면 바로 NextChatIDs 배열의 첫 번째로 진행
        if (SelectChatTexts.Num() == 0 && !Row->NextChatIDs.IsEmpty())
        {
            TArray<FString> NextIDs;
            Row->NextChatIDs.ParseIntoArray(NextIDs, TEXT("|"), true);

            if (NextIDs.Num() > 0)
            {
                CurrentID = FCString::Atoi(*NextIDs[0]);
            }
            else
            {
                EndChat();
            }
        }
    }
    else
    {
        EndChat();
    }
}

void UChatManager::SelectChatOption(int32 Index)
{
    if (!ChatDataTable) return;

    FKRChatData* Row = ChatDataTable->FindRow<FKRChatData>(*FString::FromInt(CurrentID),TEXT(""));

    if (Row)
    {
        TArray<FString> NextIDs;
        Row->NextChatIDs.ParseIntoArray(NextIDs, TEXT("|"), true);

        if (NextIDs.IsValidIndex(Index))
        {
            CurrentID = FCString::Atoi(*NextIDs[Index]);
            ShowNextChat();
        }
        else
        {
            EndChat();
        }
    }
}

void UChatManager::EndChat()
{
    UE_LOG(LogTemp, Log, TEXT("Chat Ended."));
    CurrentID = -1;

    //  여기에 챗 위젯 removefromparent , nullptr로
}
