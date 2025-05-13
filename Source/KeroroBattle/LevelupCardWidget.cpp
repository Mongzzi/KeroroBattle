// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelupCardWidget.h"
#include "KeroroPlayerController.h"
#include "KeroroGameInstance.h"
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
	if (DrawCardAnim)
	{
		AKeroroPlayerController* PC = Cast<AKeroroPlayerController>(GetOwningPlayer());
		PC->SetUIMode();

		SetCardInfo();
		PlayAnimation(DrawCardAnim);
	}
}

void ULevelupCardWidget::PlaySelectCardAnimation()
{
	if (SelectCardAnim)
	{
		PlayAnimation(SelectCardAnim);
	}
}

void ULevelupCardWidget::PlayAnotherSelectCardAnimation()
{
	if (AnotherSelectCardAnim)
	{
		PlayAnimation(AnotherSelectCardAnim);
	}
}

void ULevelupCardWidget::SetCardInfo()
{
	int32 num = FMath::RandRange(1, 76);  // 카드 ID 랜덤
	AKeroroPlayerController* PC = Cast<AKeroroPlayerController>(GetOwningPlayer());
	FCardData* CardData = PC->GetGameInstance<UKeroroGameInstance>()->GetCardData(num);

	if (CardTitle)
		CardTitle->SetText(CardData->CardName);

	if (CardDescription)
		CardDescription->SetText(CardData->Description);

	UTexture2D* Image = LoadObject<UTexture2D>(nullptr, *CardData->ImagePath.ToString());
	if (CardImage && Image)
		CardImage->SetBrushFromTexture(Image);
}

void ULevelupCardWidget::OnSelectButtonClicked()
{
	UE_LOG(LogTemp, Log, TEXT("Level Up Card Selected!"));

	// 선택된 카드 인덱스 전달
	OnCardSelected.Broadcast(CardIndex);
	AKeroroPlayerController* PC = Cast<AKeroroPlayerController>(GetOwningPlayer());

	// 여기서 카드 타입, 카드 값에 따라 플레이어 스탯 정보 업데이트해줘야함
	PC->SetGameMode();
}
