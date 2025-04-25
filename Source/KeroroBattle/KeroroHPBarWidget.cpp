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
	//NewKRStat->OnHpIsChanged.AddLambda([this]()->void{
	//	if (CurrentKRStat.IsValid())
	//	{
	//		UE_LOG(LogTemp, Error, TEXT("hhhhHPRatio : %f"), CurrentKRStat->GetHpRatio());
	//	}
	//});
}

void UKeroroHPBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UpdateHPWidget();
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
