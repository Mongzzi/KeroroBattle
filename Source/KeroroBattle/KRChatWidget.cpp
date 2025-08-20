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

void UKRChatWidget::UpdateChatInfo(int32 _ChatID, FString _ChatText, TArray<FString> _SelectChatTexts, TArray<FString> _NextChatIDs)
{
	ChatID = _ChatID;
	NextChatID1 = 0;
	NextChatID2 = 0;

	if (ChatText)
	{
		ChatText->SetText(FText::FromString(_ChatText));
	}

	// 선택지/NextIDs 모두 없는 경우 위젯 숨김
	if (_SelectChatTexts.Num() == 0 && _NextChatIDs.Num() == 0)
	{
		if (NextChatButton) NextChatButton->SetVisibility(ESlateVisibility::Collapsed);
		if (SelectButton1) SelectButton1->SetVisibility(ESlateVisibility::Collapsed);
		if (SelectButton2) SelectButton2->SetVisibility(ESlateVisibility::Collapsed);
		if (ChatText) ChatText->SetText(FText::GetEmpty());

		this->SetVisibility(ESlateVisibility::Collapsed);
		if (PC)PC->SetGameMode();
		return;
	}

	// 선택지가 있을 때
	if (_SelectChatTexts.Num() > 0 && _NextChatIDs.Num() == _SelectChatTexts.Num())
	{
		// 선택지1
		if (SelectButton1 && SelectText1)
		{
			SelectButton1->SetVisibility(ESlateVisibility::Visible);
			SelectText1->SetText(FText::FromString(_SelectChatTexts[0]));
			NextChatID1 = FCString::Atoi(*_NextChatIDs[0]);
		}

		// 선택지2
		if (SelectButton2 && SelectText2 && _SelectChatTexts.Num() > 1)
		{
			SelectButton2->SetVisibility(ESlateVisibility::Visible);
			SelectText2->SetText(FText::FromString(_SelectChatTexts[1]));
			NextChatID2 = FCString::Atoi(*_NextChatIDs[1]);
		}
		else if (SelectButton2)
		{
			SelectButton2->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (NextChatButton) NextChatButton->SetVisibility(ESlateVisibility::Collapsed);
	}
	else if (_NextChatIDs.Num() > 0)
	{
		// 선택지 없고 NextID만 있을 때
		if (NextChatButton)
		{
			NextChatButton->SetVisibility(ESlateVisibility::Visible);
			NextChatID1 = FCString::Atoi(*_NextChatIDs[0]);
		}

		if (SelectButton1) SelectButton1->SetVisibility(ESlateVisibility::Collapsed);
		if (SelectButton2) SelectButton2->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UKRChatWidget::OnSelectButton1()
{
	if (!PC || !PC->KRChatManager)return;
	UChatManager* ChatManager = PC->KRChatManager;

	if (ChatManager)
	{
		ChatManager->StartChat(NextChatID1);
	}
}

void UKRChatWidget::OnSelectButton2()
{
	if (!PC || !PC->KRChatManager)return;
	UChatManager* ChatManager = PC->KRChatManager;

	if (ChatManager)
	{
		ChatManager->StartChat(NextChatID2);
	}
}

void UKRChatWidget::OnSelectNextButton()
{
	if (!PC || !PC->KRChatManager)return;
	UChatManager* ChatManager = PC->KRChatManager;

	if (ChatManager)
	{
		ChatManager->StartChat(NextChatID1);
	}
}
