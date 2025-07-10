// Fill out your copyright notice in the Description page of Project Settings.


#include "KeroroEffectWidget.h"

void UKeroroEffectWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UKeroroEffectWidget::PlayParryWidgetEffect()
{
	if (ParryingAnim)
	{
		PlayAnimation(ParryingAnim);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("ParryAnim is nullptr"));
	}
}
