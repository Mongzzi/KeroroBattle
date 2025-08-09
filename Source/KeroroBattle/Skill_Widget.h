// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KeroroBattle.h"
#include "Blueprint/UserWidget.h"
#include "Skill_Widget.generated.h"

USTRUCT()
struct FPullingEnemy
{
	GENERATED_BODY()

	UPROPERTY()
	class AKeroroEnemyCharacter* Enemy = nullptr;

	UPROPERTY()
	FVector InitialLocation = FVector::ZeroVector;

	UPROPERTY()
	bool bHasArrived = false;
};



UCLASS()
class KEROROBATTLE_API USkill_Widget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	USkill_Widget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;
	
	void UseItem();
	void SetItem(EItemType type);
	void SetItemType(EItemType type) { ItemType = type; };
	void SetItemImage(EItemType type);
	void SetSkillImageNon();
	void SetSkillImage(int32 Image_num);
	void SetBorderImage(EWidgetType Type);
	void SetItemSlotNum(FString slot);
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

	void SetTextFromString(FString Str,FVector Color = FVector(1.0f), FVector2D Scale= FVector2D(1.0f));

	EItemType GetItemType() { return ItemType; };

private:
	float EnhanceValue_AttackUp = 50.0f;
	float EnhanceValue_SpeedUp = 500.0f;
	float EnhanceValue_DefenceUp = 0.3f;

private:
	TSubclassOf<class UDamageTextWidget> DamageTextWidgetClass;

	UPROPERTY()
	class UNiagaraComponent* PullEffectComponent;

	UPROPERTY()
	class UNiagaraSystem* PullEffect;

	UPROPERTY()
	TArray<FPullingEnemy> PulledEnemies;

	FVector PullCenter;

	FTimerHandle PullTimerHandle;
	FTimerHandle PullEffectTimerHandle;

	float PullDuration = 1.0f;
	float PullElapsed = 0.0f;

	void StartPull();
	void UpdatePull();
	void EndPull();
	void EndPullEffect();


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
