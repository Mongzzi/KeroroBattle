// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KeroroHUDWidget.generated.h"

/**
 *
 */
UCLASS()
class KEROROBATTLE_API UKeroroHUDWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;

public:
	void BindKRStat(class UKeroroStatComponent* NewKRStat);
	void BindPlayerState(class AKeroroPlayerState* PlayerState);

	void UpdateWidget();
	void UpdateHPWidget();	// ¿Ï
	void UpdateGoldWidget();
	void UpdateTimeWidget(float RemainTime);	// ¿Ï
	void UpdateEXPWidget();
	void UpdateKillWidget();

	UFUNCTION()
	void UpdateLevelWidget();
	void PlayCardAnimation(int32 SelectedIndex);
	void PlayDrawAnimation_AllCard();

private:
	TWeakObjectPtr<class UKeroroStatComponent> CurrentKRStat;
	TWeakObjectPtr<class AKeroroPlayerState> CurrentKRPlayerState;

private:

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HPBar;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* EXPBar;
	UPROPERTY(meta = (BindWidget))
	class UImage* KillImage;

	UPROPERTY(meta = (BindWidget))
	class UImage* GoldImage;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* TimeText;

	UPROPERTY(meta=(BindWidget))
	class UEditableTextBox* KillText;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* GoldText;

	UPROPERTY(meta=(BindWidget))
	class UEditableTextBox* LevelText;

public:
	UPROPERTY(meta = (BindWidget))
	class ULevelupCardWidget* CardWidget1;

	UPROPERTY(meta = (BindWidget))
	class ULevelupCardWidget* CardWidget2;

	UPROPERTY(meta = (BindWidget))
	class ULevelupCardWidget* CardWidget3;
};
