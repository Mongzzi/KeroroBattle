// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KeroroHPBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class KEROROBATTLE_API UKeroroHPBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindKRStat(class UKeroroStatComponent* NewKRStat);
	void SetHPBarText();
	void SetHPBarTextVisible();
	void SetHpBarTextHidden();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	void UpdateHPWidget();

private:
	TWeakObjectPtr<class UKeroroStatComponent> CurrentKRStat;

	UPROPERTY(meta= (BindWidget))
	class UTextBlock* HP_TEXT;

	UPROPERTY(meta= (BindWidget))
	class UProgressBar* Hp_Bar;

private:
	bool IsVisible = false;
	FTimerHandle TextVisibleHandle;
};
