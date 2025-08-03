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
	ItemType = EItemType::None;
}

void USkill_Widget::UseItem()
{
	switch (ItemType)
	{
	case EItemType::HP:
		UE_LOG(LogTemp, Error, TEXT("Use HP Item"));
		break;
	case EItemType::MP:
		break;
	case EItemType::GOLD:
		break;
	case EItemType::SKILLCOOL:
		break;
	case EItemType::GUARDCOOL:
		break;
	}
	ItemType = EItemType::None;
	SetSkillImageNon();
}

void USkill_Widget::SetItem(EItemType type)
{
	SetItemType(type);
	SetItemImage(type);
}

void USkill_Widget::SetItemImage(EItemType type)
{
	UTexture2D* Image;

	switch (type)
	{
	case EItemType::HP:
		Image = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Texture/67.67"));
		if (Image) SkillImage->SetBrushFromTexture(Image);
		break;
	case EItemType::MP:
		Image = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Texture/68.68"));
		if (Image) SkillImage->SetBrushFromTexture(Image);
		break;
	case EItemType::GOLD:
		Image = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Texture/14.14"));
		if (Image) SkillImage->SetBrushFromTexture(Image);
		break;
	case EItemType::SKILLCOOL:
		Image = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Texture/16.16"));
		if (Image) SkillImage->SetBrushFromTexture(Image);
		break;
	case EItemType::GUARDCOOL:
		Image = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Texture/17.17"));
		if (Image) SkillImage->SetBrushFromTexture(Image);
		break;
	}
	FLinearColor TransparentColor = SkillImage->ColorAndOpacity;
	TransparentColor.A = 1.0f;
	SkillImage->SetColorAndOpacity(TransparentColor);

}

void USkill_Widget::SetSkillImageNon()
{
	if (SkillImage)
	{
		SkillImage->SetBrushFromTexture(nullptr);

		FLinearColor TransparentColor = SkillImage->ColorAndOpacity;
		TransparentColor.A = 0.0f;
		SkillImage->SetColorAndOpacity(TransparentColor);
	}
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

	else if (Type == EWidgetType::ITEM_IMAGE)
	{
		FSoftObjectPath ShieldTexturePath(TEXT("/Game/Texture/ItemSlot.ItemSlot"));
		UTexture2D* Image = LoadObject<UTexture2D>(nullptr, *ShieldTexturePath.ToString());

		if (Image)
		{
			SkillBorder->SetBrushFromTexture(Image);
		}
	}
}

void USkill_Widget::SetItemSlotNum(int32 num)
{
	if (SlotName)
	{
		FString Str = FString::FromInt(num) + TEXT("번 아이템");
		SlotName->SetText(FText::FromString(Str));
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
