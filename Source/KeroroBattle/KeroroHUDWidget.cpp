// Fill out your copyright notice in the Description page of Project Settings.


#include "KeroroHUDWidget.h"
#include "KeroroStatComponent.h"
#include "KeroroPlayerState.h"
#include "LevelupCardWidget.h"
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
			// 레벨업 시 카드 드로우
			PS->OnLevelChanged.AddUObject(this, &UKeroroHUDWidget::PlayDrawAnimation_AllCard);

			// UI 업데이트
			PS->OnLevelChanged.AddUObject(this, &UKeroroHUDWidget::UpdateLevelWidget);
			PS->OnExpChanged.AddUObject(this, &UKeroroHUDWidget::UpdateEXPWidget);
			PS->OnKillNumChanged.AddUObject(this, &UKeroroHUDWidget::UpdateKillWidget);
			PS->OnGoldChanged.AddUObject(this, &UKeroroHUDWidget::UpdateGoldWidget);
		}
	}

	// 카드 인덱스 부여
	if (CardWidget1) CardWidget1->SetCardIndex(1);
	if (CardWidget2) CardWidget2->SetCardIndex(2);
	if (CardWidget3) CardWidget3->SetCardIndex(3);

	// 델리게이트 바인딩
	if (CardWidget1) CardWidget1->OnCardSelected.AddUObject(this, &UKeroroHUDWidget::PlayCardAnimation);
	if (CardWidget2) CardWidget2->OnCardSelected.AddUObject(this, &UKeroroHUDWidget::PlayCardAnimation);
	if (CardWidget3) CardWidget3->OnCardSelected.AddUObject(this, &UKeroroHUDWidget::PlayCardAnimation);

	// 처음 Draw 애니메이션 실행
	PlayDrawAnimation_AllCard();
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

void UKeroroHUDWidget::PlayCardAnimation(int32 SelectedIndex)
{
	if (CardWidget1 && CardWidget1->CardIndex == SelectedIndex)
	{
		CardWidget1->PlaySelectCardAnimation();
	}
	else if (CardWidget1)
	{
		CardWidget1->PlayAnotherSelectCardAnimation();
	}

	if (CardWidget2 && CardWidget2->CardIndex == SelectedIndex)
	{
		CardWidget2->PlaySelectCardAnimation();
	}
	else if (CardWidget2)
	{
		CardWidget2->PlayAnotherSelectCardAnimation();
	}

	if (CardWidget3 && CardWidget3->CardIndex == SelectedIndex)
	{
		CardWidget3->PlaySelectCardAnimation();
	}
	else if (CardWidget3)
	{
		CardWidget3->PlayAnotherSelectCardAnimation();
	}
}

void UKeroroHUDWidget::PlayDrawAnimation_AllCard()
{
	if (CardWidget1) CardWidget1->PlayDrawCardAnimation();
	if (CardWidget2) CardWidget2->PlayDrawCardAnimation();
	if (CardWidget3) CardWidget3->PlayDrawCardAnimation();
}

void UKeroroHUDWidget::UpdateGoldWidget()
{
	if (CurrentKRPlayerState == nullptr) return;
	if (GoldText)
	{
		GoldText->SetText(FText::FromString(FString::FromInt(CurrentKRPlayerState->CurrentGold) + TEXT("G")));
	}
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