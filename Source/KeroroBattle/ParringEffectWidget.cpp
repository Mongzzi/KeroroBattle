// Fill out your copyright notice in the Description page of Project Settings.


#include "ParringEffectWidget.h"

void UParringEffectWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UParringEffectWidget::PlayParryWidgetEffect()
{
	if (ParryingAnim)
	{
		PlayAnimation(ParryingAnim);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("ParryAnim is nullptr"));
	}
}
