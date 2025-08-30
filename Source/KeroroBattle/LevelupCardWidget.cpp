// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelupCardWidget.h"
#include "KeroroPlayerController.h"
#include "KeroroGameInstance.h"
#include "KeroroPlayerState.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "KeroroHUDWidget.h"


void ULevelupCardWidget::NativeConstruct()
{
	Super::NativeConstruct();
	KRGI = Cast<UKeroroGameInstance>(GetGameInstance());
	if (!KRGI) return;

	if (CardButton)
	{
		CardButton->OnClicked.AddDynamic(this, &ULevelupCardWidget::OnSelectButtonClicked);
		CardButton->OnHovered.AddDynamic(this, &ULevelupCardWidget::OnButtonHoverd);
	}

	CardSelectAnimFinish.BindDynamic(this, &ULevelupCardWidget::OnCardSelectAnimFinished);
	BindToAnimationFinished(SelectCardAnim, CardSelectAnimFinish);

	CardDrawAnimFinish.BindDynamic(this, &ULevelupCardWidget::OnCardDrawAnimFinished);
	BindToAnimationFinished(DrawCardAnim, CardDrawAnimFinish);

}

void ULevelupCardWidget::NativeDestruct()
{
	Super::NativeDestruct();
	if (CardButton)
	{
		CardButton->OnClicked.RemoveDynamic(this, &ULevelupCardWidget::OnSelectButtonClicked);
		CardButton->OnHovered.RemoveDynamic(this, &ULevelupCardWidget::OnButtonHoverd);
	}
	CardSelectAnimFinish.Unbind();
}

void ULevelupCardWidget::OnButtonHoverd()
{
	KRGI->PlayUISound(EUISoundType::Hover);
}

void ULevelupCardWidget::PlayDrawCardAnimation()
{
	if (DrawCardAnim)
	{
		SetCardInfo();
		PlayAnimation(DrawCardAnim);
	}
	if (CardButton)
	{
		CardButton->SetIsEnabled(false);
	}
}

void ULevelupCardWidget::PlaySelectCardAnimation()
{
	if (SelectCardAnim)
	{
		PlayAnimation(SelectCardAnim);
	}
	if (CardButton)
	{
		CardButton->SetIsEnabled(false);
	}
}

void ULevelupCardWidget::PlayAnotherSelectCardAnimation()
{
	if (AnotherSelectCardAnim)
	{
		PlayAnimation(AnotherSelectCardAnim);
	}
	if (CardButton)
	{
		CardButton->SetIsEnabled(false);
	}
}

void ULevelupCardWidget::SetCardInfo()
{
	int32 CardID = FMath::RandRange(1, 68);  // 카드 ID 랜덤
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
		UE_LOG(LogTemp, Error, TEXT("ssssssssss"))
	}
}

void ULevelupCardWidget::OnSelectButtonClicked()
{
	KRGI->PlayUISound(EUISoundType::Click);

	// 선택된 카드 인덱스 전달
	if (OnCardSelected.IsBound())
	{
		OnCardSelected.Broadcast(CardIndex);
	}
	AKeroroPlayerController* PC = Cast<AKeroroPlayerController>(GetOwningPlayer());

	AKeroroPlayerState* PS = PC->GetPlayerState<AKeroroPlayerState>();
	if (PS)
	{
		switch (CardType)
		{
		case ECardType::MaxHP:
			PS->MaxHP_Enhanced += CardValue;
			break;
		case ECardType::MaxMP:
			PS->MaxMP_Enhanced += CardValue;
			break;
		case ECardType::MoveSpeed:
			PS->MaxMoveSpeed_Enhanced += CardValue;
			break;
		case ECardType::CriticalChance:
			PS->CritChanceRate_Enhanced += CardValue;
			break;
		case ECardType::CriticalDamage:
			PS->CritDamageRate_Enhanced += CardValue;
			break;
		case ECardType::EXPBoost:
			PS->ExpGainRate += CardValue;
			break;
		case ECardType::GoldBoost:
			PS->GoldGainRate += CardValue;
			break;
		case ECardType::CooldownReduction:
			PS->SkillCooldownRate_Enhanced += CardValue;
			break;
		case ECardType::ProjectileCount:
			PS->ProjectileCount_Enhanced += CardValue;
			break;
		case ECardType::ProjectileSize:
			PS->ProjectileScale_Enhanced += CardValue;
			break;
		case ECardType::Evasion:
			PS->EvasionRate_Enhanced += CardValue;
			break;
		case ECardType::InvincibilityTime:
			PS->InvincibilityTime_Enhanced += CardValue;
			break;
		case ECardType::Armor:
			PS->DefenseRate_Enhanced += CardValue;
			break;
		case ECardType::HealAmount:
			PS->HealPowerRate_Enhanced += CardValue;
			break;
		case ECardType::HealOnKill:
			PS->HealPowerOnKill_Enhanced += CardValue;
			break;
		case ECardType::Shield:
			PS->GuardTime_Enhanced += CardValue * 3.0f;
			PS->ParryTime_Enhanced += CardValue;
			break;
		case ECardType::ShieldCoolTime:
			PS->GuardCoolTime_Enhanced += CardValue;
			break;
		case ECardType::MpHealAmount:
			PS->MpHealPowerRate_Enhanced += CardValue;
			break;
		case ECardType::MpHealOnKill:
			PS->MpHealPowerOnKill_Enhanced += CardValue;
			break;
		}
	}
	PC->UpdateStatCardEnhanced();
}

void ULevelupCardWidget::OnCardSelectAnimFinished()
{
	AKeroroPlayerController* PC =Cast<AKeroroPlayerController>(GetOwningPlayer());
	if (!PC) return;
	
	UKeroroHUDWidget* HUD = PC->KRHUDWidget;
	if (!HUD) return;

	HUD->bIsCardDrawing = false;
	
	auto& Queue = HUD->CardDrawQueue;
	if (Queue.IsEmpty())
	{
		PC->SetGameMode();
		return;
	}
	else
	{
		Queue.Pop();
		HUD->PlayDrawAnimation_AllCard();
	}
}

void ULevelupCardWidget::OnCardDrawAnimFinished()
{
	if (CardButton)
	{
		CardButton->SetIsEnabled(true);
	}
}
