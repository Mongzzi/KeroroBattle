// Fill out your copyright notice in the Description page of Project Settings.


#include "KeroroHUDWidget.h"
#include "KeroroStatComponent.h"
#include "KeroroPlayerState.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/EditableTextBox.h"

void UKeroroHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UE_LOG(LogTemp, Error, TEXT(" in NativeConstruct"));
	

	if (APlayerController* PC = GetOwningPlayer())
	{
		if (AKeroroPlayerState* PS = Cast<AKeroroPlayerState>(PC->PlayerState))
		{
			PS->OnLevelChanged.AddUObject(this, &UKeroroHUDWidget::UpdateLevelWidget);
			PS->OnExpChanged.AddUObject(this, &UKeroroHUDWidget::UpdateEXPWidget);
			PS->OnKillNumChanged.AddUObject(this, &UKeroroHUDWidget::UpdateKillWidget);
		}
	}

	if (KillText)
	{
		KillText->SetText(FText::FromString(TEXT("1234")));
	}

	if (GoldText)
	{
		GoldText->SetText(FText::FromString(TEXT("999")));
	}

	if (LevelText)
	{
		LevelText->SetText(FText::FromString(TEXT("555")));
	}

}

void UKeroroHUDWidget::UpdateHPWidget()
{
	// 프로그레스바 이미지 min 0.266 max 0.866이 0~100%처럼보임 그래서 보간해주고 SetPercent해줌
	if (CurrentKRStat == nullptr || HPBar == nullptr) return;
	float a = 0.266f + (CurrentKRStat->GetHpRatio()) * 0.6f;
	HPBar->SetPercent(a);
}

void UKeroroHUDWidget::UpdateLevelWidget()
{

	if (LevelText)
	{
		UE_LOG(LogTemp, Error, TEXT(" in level updated"));

		LevelText->SetText(FText::FromString(TEXT("LV ") + FString::FromInt(CurrentKRStat->Level)));
	}
}

void UKeroroHUDWidget::UpdateGoldWidget()
{
}

void UKeroroHUDWidget::UpdateTimeWidget(float RemainTime)
{
	if (TimeText)
	{
		TimeText->SetText(FText::FromString(FString::FromInt(RemainTime)));
	}
}

void UKeroroHUDWidget::UpdateEXPWidget()
{

	if (CurrentKRPlayerState == nullptr) return;
	float a = CurrentKRPlayerState->GetExpRatio();
	EXPBar->SetPercent(a);
}

void UKeroroHUDWidget::UpdateKillWidget()
{
	if (CurrentKRPlayerState == nullptr) return;
	if (KillText)
	{
		KillText->SetText(FText::FromString(FString::FromInt(CurrentKRPlayerState->KilledEnemyNum)));
	}
}

void UKeroroHUDWidget::BindKRStat(UKeroroStatComponent* NewKRStat)
{
	if (NewKRStat == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("NewKRStat is nullptr in hud widget"));
		return;
	}

	CurrentKRStat = NewKRStat;

	UpdateHPWidget();


}

void UKeroroHUDWidget::BindPlayerState(AKeroroPlayerState* PlayerState)
{
	if (PlayerState == nullptr) return;
	CurrentKRPlayerState = PlayerState;
	UpdateLevelWidget();
	UpdateEXPWidget();
	UpdateKillWidget();
}

void UKeroroHUDWidget::UpdateWidget()
{
}

void UKeroroHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

}