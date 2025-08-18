// Fill out your copyright notice in the Description page of Project Settings.


#include "KRChatWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UKRChatWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UKRChatWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UKRChatWidget::ChangeText(FString str)
{
	if (!ChatText)return;

	ChatText->SetText(FText::FromString(str));
}
