// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KeroroBattle.h"
#include "Blueprint/UserWidget.h"
#include "KRUnlockWidget.generated.h"

/**
 * 
 */
UCLASS()
class KEROROBATTLE_API UKRUnlockWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void ChangeImage(EKeroroType type);
	void ChangeText(EKeroroType type);


protected:
	UPROPERTY(meta = (BindWidget))
	class UImage* UnlockKeroImage;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* UnlockKeroText;
};
