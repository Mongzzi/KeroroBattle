// Fill out your copyright notice in the Description page of Project Settings.


#include "KeroroHUDWidget.h"
#include "KeroroStatComponent.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/EditableTextBox.h"

void UKeroroHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UE_LOG(LogTemp, Error, TEXT(" in NativeConstruct"));


	if (KillText)
	{
		KillText->SetText(FText::FromString(TEXT("1234")));
	}

	if (GoldText)
	{
		GoldText->SetText(FText::FromString(TEXT("999")));
	}


}

void UKeroroHUDWidget::UpdateHPWidget()
{
	// 프로그레스바 이미지 min 0.266 max 0.866이 0~100%처럼보임 그래서 보간해주고 SetPercent해줌
	float a = 0.266f + (CurrentKRStat->GetHpRatio())*0.6f;
	HPBar->SetPercent(a);
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
}

void UKeroroHUDWidget::UpdateKillWidget()
{
}

void UKeroroHUDWidget::BindKRStat(UKeroroStatComponent* NewKRStat)
{
	if (NewKRStat == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("NewKRStat is nullptr in hud widget"));
		return;
	}

	CurrentKRStat = NewKRStat;

	//// 추후 작업 예정
	//NewKRStat->OnHpIsChanged.AddUObject(this, &UKeroroHUDWidget::UpdateHPWidget);
	//UpdateHPWidget();
}

void UKeroroHUDWidget::UpdateWidget()
{
}

void UKeroroHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

}