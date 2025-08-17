// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KeroroBattle.h"
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
	virtual void NativeDestruct() override;
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void BindKRStat(class UKeroroStatComponent* NewKRStat);
	void BindPlayerState(class AKeroroPlayerState* PlayerState);

	void UpdateWidget();
	void UpdateHPWidget();
	void UpdateMPWidget();
	void UpdateGoldWidget();
	void UpdateTimeWidget(float RemainTime);
	void UpdateEXPWidget();
	void UpdateKillWidget();
	void UpdateSkillCoolTimeWidget();
	void ChangeSkillImage();

	void UseItem(int32 slot_num);
	void ChangeItemImage(int32 slot_num,EItemType type);

	UFUNCTION()
	void UpdateLevelWidget();
	void PlayCardAnimation(int32 SelectedIndex);
	void PlayDrawAnimation_AllCard();

public:
	bool bIsCardDrawing = false;
	TArray<int32>CardDrawQueue;
	
private:
	TWeakObjectPtr<class UKeroroStatComponent> CurrentKRStat;
	TWeakObjectPtr<class AKeroroPlayerState> CurrentKRPlayerState;

private:

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HPBar;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* MPBar;
	
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

	UPROPERTY(meta = (BindWidget))
	class USkill_Widget* GuardWidget;

	UPROPERTY(meta= (BindWidget))
	class USkill_Widget* SkillWidget;

	UPROPERTY(meta = (BindWidget))
	class UReRollButton* ReRollWidget;

public:
	UPROPERTY(meta = (BindWidget))
	class ULevelupCardWidget* CardWidget1;

	UPROPERTY(meta = (BindWidget))
	class ULevelupCardWidget* CardWidget2;

	UPROPERTY(meta = (BindWidget))
	class ULevelupCardWidget* CardWidget3;

	UPROPERTY(meta = (BindWidget))
	class USkill_Widget* ItemWidget1;

	UPROPERTY(meta = (BindWidget))
	class USkill_Widget* ItemWidget2;

	UPROPERTY(meta = (BindWidget))
	class USkill_Widget* ItemWidget3;

};
