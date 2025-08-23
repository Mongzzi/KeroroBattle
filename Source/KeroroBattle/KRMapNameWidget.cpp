// Fill out your copyright notice in the Description page of Project Settings.


#include "KRMapNameWidget.h"
#include "Components/TextBlock.h"
#include "KeroroGameInstance.h"
#include "KeroroPlayerController.h"
#include "Kismet/GameplayStatics.h"

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

	FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);


	if (CurrentLevelName == TEXT("Robby1Level"))
	{
		MapName->SetText(FText::FromString(TEXT("케론별 유적지")));

	}
	else if (CurrentLevelName == TEXT("MainLevel1"))
	{
		MapName->SetText(FText::FromString(TEXT("케론별 뒷산")));

	}
	else if (CurrentLevelName == TEXT("MainLevel2"))
	{
		MapName->SetText(FText::FromString(TEXT("케론별 피라미드")));
	}
	else if (CurrentLevelName == TEXT("MainLevel3"))
	{
		EKeroroType RoundType = GI->NextMissionRound;
		if (RoundType == EKeroroType::Dororo)
		{
			MapName->SetText(FText::FromString(TEXT("케론별 궁전")));
		}
		else
		{
			MapName->SetText(FText::FromString(TEXT("케론별 버려진 땅")));
		}
	}
}

void UKRMapNameWidget::PlayEntraceAnim()
{
	if (EntraceAnim)
	{
		PlayAnimation(EntraceAnim);
	}
}