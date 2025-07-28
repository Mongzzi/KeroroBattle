// Fill out your copyright notice in the Description page of Project Settings.


#include "KeroroHPBarWidget.h"
#include "KeroroStatComponent.h"
#include "Components/ProgressBar.h"

void UKeroroHPBarWidget::BindKRStat(UKeroroStatComponent* NewKRStat)
{
	if (NewKRStat == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("NewKRStat is nullptr"));
		return;
	}

	CurrentKRStat = NewKRStat;
	NewKRStat->OnHpIsChanged.AddUObject(this, &UKeroroHPBarWidget::UpdateHPWidget);
}

void UKeroroHPBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UpdateHPWidget();
}

void UKeroroHPBarWidget::NativeDestruct()
{
	if (CurrentKRStat.IsValid())
	{
		CurrentKRStat->OnHpIsChanged.RemoveAll(this);
	}

	Super::NativeDestruct();
}

void UKeroroHPBarWidget::UpdateHPWidget()
{
	if (CurrentKRStat.IsValid())
	{
		if (Hp_Bar != nullptr)
		{
			Hp_Bar->SetPercent(CurrentKRStat->GetHpRatio());
		}
	}
}
