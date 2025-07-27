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
	void SetTextFromDamage(float Damage);
	void SetTextFromCritDamage(float Damage);
	void SetTextMiss();
	void SetTextGuard();
	void SetTextParry();
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
