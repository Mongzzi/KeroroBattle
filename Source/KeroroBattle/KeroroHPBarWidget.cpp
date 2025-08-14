// Fill out your copyright notice in the Description page of Project Settings.


#include "KeroroHPBarWidget.h"
#include "KeroroStatComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

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

void UKeroroHPBarWidget::SetHPBarTextVisibleTimer()
{
	IsVisible = true;
	UpdateHPWidget();

	GetWorld()->GetTimerManager().ClearTimer(TextVisibleHandle);
	GetWorld()->GetTimerManager().SetTimer(TextVisibleHandle, this, &UKeroroHPBarWidget::SetHpBarTextHidden, 5.0f);
}

void UKeroroHPBarWidget::SetHpBarTextHidden()
{
	IsVisible = false;
	UpdateHPWidget();
}

void UKeroroHPBarWidget::SetHPBarTextVisible()
{
	IsVisible = true;
	UpdateHPWidget();
}

void UKeroroHPBarWidget::SetHPBarText()
{
	if (!CurrentKRStat.IsValid() || !HP_TEXT) return;

	if (IsVisible)
	{
		float CurHp = CurrentKRStat->CurrentHp;
		float MaxHp = CurrentKRStat->MaxHp;
		FString HPText = FString::Printf(TEXT("%.0f / %.0f"), CurHp, MaxHp);
		HP_TEXT->SetText(FText::FromString(HPText));
		HP_TEXT->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		HP_TEXT->SetVisibility(ESlateVisibility::Hidden);
	}
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
	GetWorld()->GetTimerManager().ClearTimer(TextVisibleHandle);

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

		if (HP_TEXT != nullptr)
		{
			SetHPBarText();
		}
	}
}
