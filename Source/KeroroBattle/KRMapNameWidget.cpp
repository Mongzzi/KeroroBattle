// Fill out your copyright notice in the Description page of Project Settings.


#include "KRMapNameWidget.h"
#include "Components/TextBlock.h"
#include "KeroroGameInstance.h"
#include "KeroroPlayerController.h"

void UKRMapNameWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetMapNameText();
	PlayEntraceAnim();
}

void UKRMapNameWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UKRMapNameWidget::SetMapNameText()
{
	UKeroroGameInstance* GI = GetGameInstance<UKeroroGameInstance>();
	AKeroroPlayerController* PC = Cast<AKeroroPlayerController>(GetOwningPlayer());
	if (!GI || !PC || !MapName)return;
	if (!PC->IsMainMap) return;

	EKeroroType RoundType = GI->NextMissionRound;

	switch (RoundType)
	{
	case EKeroroType::Keroro:
		MapName->SetText(FText::FromString(TEXT("케론별 버려진 땅")));
		break;
	case EKeroroType::Tamama:
		MapName->SetText(FText::FromString(TEXT("케론별 뒷산")));
		break;
	case EKeroroType::Giroro:
		MapName->SetText(FText::FromString(TEXT("케론별 뒷산")));

		break;
	case EKeroroType::Kururu:
		MapName->SetText(FText::FromString(TEXT("케론별 유적지")));

		break;
	case EKeroroType::Dororo:
		MapName->SetText(FText::FromString(TEXT("케론별 궁전")));
		break;
	}
}

void UKRMapNameWidget::PlayEntraceAnim()
{
	if (EntraceAnim)
	{
		PlayAnimation(EntraceAnim);
	}
}