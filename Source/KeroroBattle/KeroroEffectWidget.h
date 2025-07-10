// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KeroroEffectWidget.generated.h"

/**
 * 
 */
UCLASS()
class KEROROBATTLE_API UKeroroEffectWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;
	void PlayParryWidgetEffect();


protected:
	
	UPROPERTY(meta=(BindWidgetAnim),Transient)
	UWidgetAnimation* ParryingAnim;

};
