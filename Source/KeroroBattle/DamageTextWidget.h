// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageTextWidget.generated.h"

/**
 * 
 */
UCLASS()
class KEROROBATTLE_API UDamageTextWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void SetTextFromString(FString Str, FVector Color = FVector(1.0f), FVector2D Scale = FVector2D(1.0f));
	void SetTextFromDamage(float Damage);
	void SetTextFromCritDamage(float Damage);
	void SetTextMiss();
	void SetTextGuard();
	void SetTextHeal(float Hp, float Mp);
	void SetTextParry();
	void SetTextinvincible();
	void SetTargetLocation(FVector Loc);
	void PlayFadeOutAnimation();

private: 
	FVector TargetLocation;
	FVector2D RandScreenOffset;

	float CurrentLerpTime;
	float TotalLerpDuration;

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DamageText;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* FadeOutAnimation;
};
