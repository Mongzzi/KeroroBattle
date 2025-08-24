// Fill out your copyright notice in the Description page of Project Settings.


#include "ReRollButton.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "KeroroPlayerController.h"
#include "KeroroPlayerState.h"
#include "KeroroGameInstance.h"

void UReRollButton::NativeConstruct()
{
	Super::NativeConstruct();
	KRGI = Cast<UKeroroGameInstance>(GetGameInstance());
	if (!KRGI) return;

	if (RerollButton)
	{
		RerollButton->OnClicked.AddDynamic(this, &UReRollButton::OnSelectButtonClicked);
		RerollButton->OnHovered.AddDynamic(this, &UReRollButton::OnButtonHoverd);
	}
}

void UReRollButton::NativeDestruct()
{
	Super::NativeDestruct();
	if (RerollButton)
	{
		RerollButton->OnClicked.RemoveDynamic(this, &UReRollButton::OnSelectButtonClicked);
		RerollButton->OnHovered.RemoveDynamic(this, &UReRollButton::OnButtonHoverd);
	}
}

void UReRollButton::SetGoldText(int32 InGold)
{
	if (RerollText)
	{
		FString Str = FString::Printf(TEXT("ReRoll %dG"), InGold);
		RerollText->SetText(FText::FromString(Str));
	}
}

void UReRollButton::PlayFadeInAnim()
{
	if (FadeIn)
	{
		PlayAnimation(FadeIn);
	}
}

void UReRollButton::PlayFadeOutAnim()
{
	if (FadeOut)
	{
		PlayAnimation(FadeOut);
	}
}

void UReRollButton::OnSelectButtonClicked()
{
	KRGI->PlayUISound(EUISoundType::Click);
	AKeroroPlayerController* PC = Cast<AKeroroPlayerController>(GetOwningPlayer());
	if (PC)
	{
		if (AKeroroPlayerState* PS = Cast<AKeroroPlayerState>(PC->PlayerState))
		{
			bool CanBuy=PS->PayGold(100);
			if (!CanBuy) return;
		}
	}

	if (OnRerollButtonSelected.IsBound())
	{
		OnRerollButtonSelected.Broadcast();
	}
}

void UReRollButton::OnButtonHoverd()
{
	KRGI->PlayUISound(EUISoundType::Hover);
}