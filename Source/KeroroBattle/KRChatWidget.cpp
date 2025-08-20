// Fill out your copyright notice in the Description page of Project Settings.


#include "KRChatWidget.h"
#include "KeroroPlayerController.h"
#include "ChatManager.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"


void UKRChatWidget::NativeConstruct()
{
	Super::NativeConstruct();

    SetVisibility(ESlateVisibility::Collapsed);

	PC = Cast<AKeroroPlayerController>(GetOwningPlayer());

	if (PC.IsValid() && PC->KRChatManager)
	{
		ChatManager = PC->KRChatManager;
		ChatManager->OnChatUpdate.AddDynamic(this, &UKRChatWidget::UpdateChatInfo);
	}

	if (NextChatButton)
	{
		NextChatButton->OnClicked.AddDynamic(this, &UKRChatWidget::OnSelectNextButton);
	}
	if (SelectButton1)
	{
		SelectButton1->OnClicked.AddDynamic(this, &UKRChatWidget::OnSelectButton1);
	}
	if (SelectButton2)
	{
		SelectButton2->OnClicked.AddDynamic(this, &UKRChatWidget::OnSelectButton2);
	}
}

void UKRChatWidget::NativeDestruct()
{
	if (ChatManager.IsValid())
	{
		ChatManager->OnChatUpdate.RemoveDynamic(this, &UKRChatWidget::UpdateChatInfo);
	}
	if (NextChatButton)
	{
		NextChatButton->OnClicked.RemoveDynamic(this, &UKRChatWidget::OnSelectNextButton);
	}
	if (SelectButton1)
	{
		SelectButton1->OnClicked.RemoveDynamic(this, &UKRChatWidget::OnSelectButton1);
	}
	if (SelectButton2)
	{
		SelectButton2->OnClicked.RemoveDynamic(this, &UKRChatWidget::OnSelectButton2);
	}
	Super::NativeDestruct();
}

void UKRChatWidget::UpdateChatInfo(int32 chatid, FString chattext, TArray<FString> SelectChatTexts, TArray<FString> nextchatids)
{
    ChatID = chatid;
    NextChatID1 = 0;
    NextChatID2 = 0;

    if (ChatText)
    {
        ChatText->SetText(FText::FromString(chattext));
    }

    // 선택지/NextIDs 모두 없는 경우 위젯 숨김
    if (SelectChatTexts.Num() == 0 && nextchatids.Num() == 0)
    {
        if (NextChatButton) NextChatButton->SetVisibility(ESlateVisibility::Collapsed);
        if (SelectButton1) SelectButton1->SetVisibility(ESlateVisibility::Collapsed);
        if (SelectButton2) SelectButton2->SetVisibility(ESlateVisibility::Collapsed);
        if (ChatText) ChatText->SetText(FText::GetEmpty());

        this->SetVisibility(ESlateVisibility::Collapsed);
        return;
    }

    // 선택지가 있을 때
    if (SelectChatTexts.Num() > 0 && nextchatids.Num() == SelectChatTexts.Num())
    {
        // 선택지1
        if (SelectButton1 && SelectText1)
        {
            SelectButton1->SetVisibility(ESlateVisibility::Visible);
            SelectText1->SetText(FText::FromString(SelectChatTexts[0]));
            NextChatID1 = FCString::Atoi(*nextchatids[0]);
        }

        // 선택지2
        if (SelectButton2 && SelectText2 && SelectChatTexts.Num() > 1)
        {
            SelectButton2->SetVisibility(ESlateVisibility::Visible);
            SelectText2->SetText(FText::FromString(SelectChatTexts[1]));
            NextChatID2 = FCString::Atoi(*nextchatids[1]);
        }
        else if (SelectButton2)
        {
            SelectButton2->SetVisibility(ESlateVisibility::Collapsed);
        }

        if (NextChatButton) NextChatButton->SetVisibility(ESlateVisibility::Collapsed);
    }
    else if (nextchatids.Num() > 0)
    {
        // 선택지 없고 NextID만 있을 때
        if (NextChatButton)
        {
            NextChatButton->SetVisibility(ESlateVisibility::Visible);
            NextChatID1 = FCString::Atoi(*nextchatids[0]);
        }

        if (SelectButton1) SelectButton1->SetVisibility(ESlateVisibility::Collapsed);
        if (SelectButton2) SelectButton2->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void UKRChatWidget::OnSelectButton1()
{
	if (ChatManager.IsValid())
	{
		ChatManager->StartChat(NextChatID1);
	}
}

void UKRChatWidget::OnSelectButton2()
{
	if (ChatManager.IsValid())
	{
		ChatManager->StartChat(NextChatID2);
	}
}

void UKRChatWidget::OnSelectNextButton()
{
	if (ChatManager.IsValid())
	{
		ChatManager->StartChat(NextChatID1);
	}
}
