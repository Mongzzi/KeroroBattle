// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelupCardWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"

void ULevelupCardWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (CardButton)
	{
		CardButton->OnClicked.AddDynamic(this, &ULevelupCardWidget::OnSelectButtonClicked);
	}
}

void ULevelupCardWidget::PlayDrawCardAnimation()
{
	if (DrawCard)
	{
		PlayAnimation(DrawCard);
	}
}

void ULevelupCardWidget::PlaySelectCardAnimation()
{
	if (SelectCard)
	{
		PlayAnimation(SelectCard);
	}
}

void ULevelupCardWidget::PlayAnotherSelectCardAnimation()
{
	if (AnotherSelectCard)
	{
		PlayAnimation(AnotherSelectCard);
	}
}

void ULevelupCardWidget::SetCardInfo(const FText& Title, const FText& Description, UTexture2D* Image)
{
	if (CardTitle)
		CardTitle->SetText(Title);

	if (CardDescription)
		CardDescription->SetText(Description);

	if (CardImage && Image)
		CardImage->SetBrushFromTexture(Image);
}

void ULevelupCardWidget::OnSelectButtonClicked()
{
	UE_LOG(LogTemp, Log, TEXT("Level Up Card Selected!"));
	
	// 선택된 카드 인덱스 전달
	OnCardSelected.Broadcast(CardIndex); 

}
