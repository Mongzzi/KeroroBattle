// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill_Widget.h"
#include "KeroroGameInstance.h"
#include "KeroroPlayerController.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"


void USkill_Widget::NativeConstruct()
{
	Super::NativeConstruct();

}

void USkill_Widget::SetSkillImage(int32 Image_num)
{
	AKeroroPlayerController* PC = Cast<AKeroroPlayerController>(GetOwningPlayer());
	FCardData* CardData = PC->GetGameInstance<UKeroroGameInstance>()->GetCardData(Image_num);

	if (CardData)
	{
		UTexture2D* Image = LoadObject<UTexture2D>(nullptr, *CardData->ImagePath.ToString());
		if (SkillImage && Image)
		{
			SkillImage->SetBrushFromTexture(Image);
		}

	}
	else {
		UE_LOG(LogTemp, Error, TEXT("CardData is nullptr"));
	}
}

void USkill_Widget::SetBorderImage(EWidgetType Type)
{
	if (!SkillBorder) {
		UE_LOG(LogTemp, Error, TEXT("SkillBorder is nullptr"));
		return;
	}

	if (Type == EWidgetType::SHIELD_IMAGE)
	{
		FSoftObjectPath ShieldTexturePath(TEXT("/Game/Blueprints/ShieldBorder.ShieldBorder"));
		UTexture2D* Image = LoadObject<UTexture2D>(nullptr, *ShieldTexturePath.ToString());

		if (Image)
		{
			SkillBorder->SetBrushFromTexture(Image);
		}
	}
}

void USkill_Widget::UpdateCoolTimeText(float Time)
{
	if (CoolTimeText)
	{
		if (Time < 0.0f)
		{
			CoolTimeText->SetText(FText::GetEmpty());
		}
		else
		{
			CoolTimeText->SetText(FText::AsNumber(static_cast<int32>(Time)));
		}
	}
}

void USkill_Widget::UpdateCoolTimeProgressBar(float Percent)
{
	if (CoolTImeProgressBar)
	{
		CoolTImeProgressBar->SetPercent(Percent);
	}
}
