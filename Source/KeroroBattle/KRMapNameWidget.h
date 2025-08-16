// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KeroroBattle.h"
#include "Blueprint/UserWidget.h"
#include "KRMapNameWidget.generated.h"

/**
 * 
 */
UCLASS()
class KEROROBATTLE_API UKRMapNameWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	void SetMapNameText();
	void PlayEntraceAnim();

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MapName;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* EntraceAnim;
};
