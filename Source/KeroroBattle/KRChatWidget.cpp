// Fill out your copyright notice in the Description page of Project Settings.


#include "KRChatWidget.h"
#include "KeroroPlayerController.h"
#include "ChatManager.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "KeroroGameInstance.h"

void UKRChatWidget::NativeConstruct()
{
	Super::NativeConstruct();
	KRGI = Cast<UKeroroGameInstance>(GetGameInstance());
	if (!KRGI) return;

	SetVisibility(ESlateVisibility::Collapsed);

	PC = Cast<AKeroroPlayerController>(GetOwningPlayer());

	if (NextChatButton)
	{
		NextChatButton->OnClicked.AddDynamic(this, &UKRChatWidget::OnSelectNextButton);
		NextChatButton->OnHovered.AddDynamic(this, &UKRChatWidget::OnButtonHoverd);
	}
	if (SelectButton1)
	{
		SelectButton1->OnClicked.AddDynamic(this, &UKRChatWidget::OnSelectButton1);
		SelectButton1->OnHovered.AddDynamic(this, &UKRChatWidget::OnButtonHoverd);
	}
	if (SelectButton2)
	{
		SelectButton2->OnClicked.AddDynamic(this, &UKRChatWidget::OnSelectButton2);
		SelectButton2->OnHovered.AddDynamic(this, &UKRChatWidget::OnButtonHoverd);
	}
}

void UKRChatWidget::NativeDestruct()
{
	if (NextChatButton)
	{
		NextChatButton->OnClicked.RemoveDynamic(this, &UKRChatWidget::OnSelectNextButton);
		NextChatButton->OnHovered.RemoveDynamic(this, &UKRChatWidget::OnButtonHoverd);
	}
	if (SelectButton1)
	{
		SelectButton1->OnClicked.RemoveDynamic(this, &UKRChatWidget::OnSelectButton1);
		SelectButton1->OnHovered.RemoveDynamic(this, &UKRChatWidget::OnButtonHoverd);
	}
	if (SelectButton2)
	{
		SelectButton2->OnClicked.RemoveDynamic(this, &UKRChatWidget::OnSelectButton2);
		SelectButton2->OnHovered.RemoveDynamic(this, &UKRChatWidget::OnButtonHoverd);
	}
	Super::NativeDestruct();
}

void UKRChatWidget::OnButtonHoverd()
{
	KRGI->PlayUISound(EUISoundType::Hover);
}

void UKRChatWidget::UpdateChatInfo(int32 _ChatID, int32 _ImageID, FString _ChatText, TArray<FString> _SelectChatTexts, TArray<FString> _NextChatIDs)
{
	ChatID = _ChatID;
	NextChatID1 = 0;
	NextChatID2 = 0;
	
	if (ImageID != _ImageID)
	{
		ImageID = _ImageID;
		if (CharacterImage)
		{
			UTexture2D* Image = LoadObject<UTexture2D>(nullptr, *FString::Printf(TEXT("/Game/Texture/%d.%d"), _ImageID, _ImageID));
			CharacterImage->SetBrushFromTexture(Image);
		}
	}

	if (ChatText)
	{
		ChatText->SetText(FText::FromString(_ChatText));
	}

	// 선택지/NextIDs 모두 없는 경우 위젯 숨김
	if (_SelectChatTexts.Num() == 0 && _NextChatIDs.Num() == 0)
	{
		if (NextChatButton)
		{
			NextChatButton->SetVisibility(ESlateVisibility::Visible);
			NextChatID1 = 0;
		}
		if (SelectButton1) SelectButton1->SetVisibility(ESlateVisibility::Collapsed);
		if (SelectButton2) SelectButton2->SetVisibility(ESlateVisibility::Collapsed);
		
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

	KRGI->PlayUISound(EUISoundType::Click);

	UChatManager* ChatManager = PC->KRChatManager;

	if (ChatManager)
	{
		ChatManager->StartChat(NextChatID1);
	}
}

void UKRChatWidget::OnSelectButton2()
{
	if (!PC || !PC->KRChatManager)return;

	KRGI->PlayUISound(EUISoundType::Click);

	UChatManager* ChatManager = PC->KRChatManager;

	if (ChatManager)
	{
		ChatManager->StartChat(NextChatID2);
	}
}

void UKRChatWidget::OnSelectNextButton()
{
	if (!PC || !PC->KRChatManager)return;

	KRGI->PlayUISound(EUISoundType::Click);
	
	UChatManager* ChatManager = PC->KRChatManager;

	if (NextChatID1 <= 0)
	{
		this->SetVisibility(ESlateVisibility::Collapsed);
		if (PC) PC->SetGameMode();
		return;
	}

	if (ChatManager)
	{
		ChatManager->StartChat(NextChatID1);
	}
}

void UKRChatWidget::PlayFadeInAnim()
{
	if (FadeIn)
	{
		PlayAnimation(FadeIn);
	}
}
