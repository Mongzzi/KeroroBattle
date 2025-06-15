// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelupCardWidget.h"
#include "KeroroPlayerController.h"
#include "KeroroGameInstance.h"
#include "KeroroPlayerState.h"
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
	int32 CardID = FMath::RandRange(1, 68);  // 카드 ID 랜덤	 카드 타입에따라 4개씩 존재 즉 17개 종류
	AKeroroPlayerController* PC = Cast<AKeroroPlayerController>(GetOwningPlayer());
	FCardData* CardData = PC->GetGameInstance<UKeroroGameInstance>()->GetCardData(CardID);

	if (CardData)
	{
		if (CardTitle)
			CardTitle->SetText(CardData->CardName);

		if (CardDescription)
			CardDescription->SetText(CardData->Description);
	
		UTexture2D* Image = LoadObject<UTexture2D>(nullptr, *CardData->ImagePath.ToString());
		if (CardImage && Image)
		{
			CardImage->SetBrushFromTexture(Image);
		}

		CardType = CardData->CardType;
		CardValue = CardData->CardValue;
	}
	else {
		UE_LOG(LogTemp,Error,TEXT("ssssssssss"))
	}
}

void ULevelupCardWidget::OnSelectButtonClicked()
{
	UE_LOG(LogTemp, Log, TEXT("Level Up Card Selected!"));

	// 선택된 카드 인덱스 전달
	OnCardSelected.Broadcast(CardIndex);
	AKeroroPlayerController* PC = Cast<AKeroroPlayerController>(GetOwningPlayer());

	// 여기서 카드 타입, 카드 값에 따라 플레이어 스탯 정보 업데이트해줘야함
	AKeroroPlayerState* PS = PC->GetPlayerState<AKeroroPlayerState>();
	if (PS)
	{
		switch (CardType)
		{
		case ECardType::MaxHP:
			PS->MaxHP_Enhanced += CardValue;
			break;
		case ECardType::MaxMP:
			break;
		case ECardType::AttackSpeed:
			break;
		case ECardType::MoveSpeed:
			break;
		case ECardType::CriticalChance:
			break;
		case ECardType::CriticalDamage:
			break;
		case ECardType::EXPBoost:
			break;
		case ECardType::GoldBoost:
			break;
		case ECardType::CooldownReduction:
			break;
		case ECardType::ProjectileCount:
			break;
		case ECardType::ProjectileSize:
			break;
		case ECardType::Evasion:
			break;
		case ECardType::InvincibilityTime:
			break;
		case ECardType::Armor:
			break;
		case ECardType::HealAmount:
			break;
		case ECardType::HealOnKill:
			break;
		case ECardType::ShieldRegen:
			break;
		case ECardType::MAX:
			break;
		default:
			break;
		}
	}
	PC->UpdateStatCardEnhanced();
	PC->SetGameMode();
}
