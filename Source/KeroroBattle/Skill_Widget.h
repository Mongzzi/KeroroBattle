// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KeroroBattle.h"
#include "Blueprint/UserWidget.h"
#include "Skill_Widget.generated.h"

/**
 * 
 */
UCLASS()
class KEROROBATTLE_API USkill_Widget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
	void UseItem();
	void SetItem(EItemType type);
	void SetItemType(EItemType type) { ItemType = type; };
	void SetItemImage(EItemType type);
	void SetSkillImageNon();
	void SetSkillImage(int32 Image_num);
	void SetBorderImage(EWidgetType Type);
	void SetItemSlotNum(int32 num);
	void UpdateCoolTimeText(float Time);
	void UpdateCoolTimeProgressBar(float Percent);
	
	void ItemHP();
	void ItemMP();
	void ItemShieldCoolZero();
	void ItemUltCoolZero();
	void ItemGold();
	void ItemKillAllEnemies();
	void ItemGroupEnemies();

	void ItemLevelUp();
	void ItemAttackUp();
	void ItemMoveSpeedUp();
	void ItemDefenseUp();

	void EndAttackUp();
	void EndSpeedUp();
	void EndDefenceUp();

	EItemType GetItemType() { return ItemType; };

private:
	float EnhanceValue_AttackUp = 50.0f;
	float EnhanceValue_SpeedUp = 500.0f;
	float EnhanceValue_DefenceUp = 0.3f;

protected:
	EItemType ItemType;

	UPROPERTY(meta=(BindWidget))
	class UImage* SkillImage;

	UPROPERTY(meta=(BindWidget))
	class UImage* SkillBorder;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CoolTimeText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SlotName;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* CoolTImeProgressBar;

};
