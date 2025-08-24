// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KeroroBattle.h"
#include "Blueprint/UserWidget.h"
#include "ReRollButton.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnRerollButtonSelected);

/**
 * 
 */
UCLASS()
class KEROROBATTLE_API UReRollButton : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	FOnRerollButtonSelected OnRerollButtonSelected;

	void SetGoldText(int32 InGold);
	void PlayFadeInAnim();
	void PlayFadeOutAnim();

public:
	UPROPERTY()
	class UKeroroGameInstance* KRGI;

	UFUNCTION()
	void OnButtonHoverd();


private:
	UPROPERTY(meta = (BindWidgetAnim),Transient)
	UWidgetAnimation* FadeIn;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* FadeOut;

	UPROPERTY(meta = (BindWidget))
	class UButton* RerollButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* RerollText;


	UFUNCTION()
	void OnSelectButtonClicked();

};
