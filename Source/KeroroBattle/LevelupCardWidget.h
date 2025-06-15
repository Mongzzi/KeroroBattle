// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KeroroBattle.h"
#include "Blueprint/UserWidget.h"
#include "LevelupCardWidget.generated.h"

// broadcast에 int32 인자로 카드인덱스 넘겨서 바인딩된 함수에 현재 인덱스 알림
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCardSelected, int32);

/**
 * 
 */
UCLASS()
class KEROROBATTLE_API ULevelupCardWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void PlayDrawCardAnimation();
	void PlaySelectCardAnimation();
	void PlayAnotherSelectCardAnimation();

	void SetCardInfo();
	void SetCardIndex(int32 Index) { CardIndex = Index; }
	FOnCardSelected OnCardSelected;

public:
	UPROPERTY()
	int32 CardIndex;

	UPROPERTY(BlueprintReadOnly,Category=Card)
	ECardType CardType = ECardType::MAX;
	float CardValue;

protected:

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* DrawCardAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* SelectCardAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* AnotherSelectCardAnim;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CardTitle;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CardDescription;

	UPROPERTY(meta = (BindWidget))
	class UImage* CardImage;

	UPROPERTY(meta = (BindWidget))
	class UButton* CardButton;

	UFUNCTION()
	void OnSelectButtonClicked();
};
