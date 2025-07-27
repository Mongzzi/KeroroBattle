// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageTextWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UDamageTextWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CurrentLerpTime = 0.0f;
	TotalLerpDuration = 1.0f;
}

void UDamageTextWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!GetOwningPlayer()) return;

	FVector2D ScreenPos;
	if (UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), TargetLocation, ScreenPos))
	{
		CurrentLerpTime += InDeltaTime;
		float Alpha = FMath::Clamp(CurrentLerpTime / TotalLerpDuration, 0.0f, 1.0f);

		FVector2D Offset = FMath::Lerp(FVector2D::ZeroVector, RandScreenOffset, Alpha);
		SetPositionInViewport(ScreenPos + Offset, true);
	}

	if (CurrentLerpTime >= TotalLerpDuration)
	{
		RemoveFromParent();
	}
}

void UDamageTextWidget::SetTextFromDamage(float Damage)
{
	if (DamageText)
	{
		DamageText->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f)));
		DamageText->SetText(FText::AsNumber(Damage));
	}
}

void UDamageTextWidget::SetTextFromCritDamage(float Damage)
{
	if (DamageText)
	{
		DamageText->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 0.05f, 0.0f)));
		DamageText->SetRenderScale(FVector2D(1.3f));
		DamageText->SetText(FText::AsNumber(Damage));
	}
}

void UDamageTextWidget::SetTextMiss()
{
	if (DamageText)
	{
		DamageText->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 0.5f, 0.0f)));
		DamageText->SetRenderScale(FVector2D(1.5f));
		DamageText->SetText(FText::FromString("MISS"));
	}
}

void UDamageTextWidget::SetTextGuard()
{
	if (DamageText)
	{
		DamageText->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f)));
		DamageText->SetRenderScale(FVector2D(1.5f));
		DamageText->SetText(FText::FromString("GUARD"));
	}
}

void UDamageTextWidget::SetTextParry()
{
	if (DamageText)
	{
		DamageText->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f)));
		DamageText->SetRenderScale(FVector2D(1.5f));
		DamageText->SetText(FText::FromString("GUARD"));
	}
}

void UDamageTextWidget::SetTargetLocation(FVector Loc)
{
	Loc += FVector(0.0f, 0.0f, 100.0f); // ¸Ó¸® À§
	TargetLocation = Loc;

	float RandomX = FMath::RandRange(-40.0f, 40.0f);
	float RandomY = FMath::RandRange(-100.0f, -50.0f);
	RandScreenOffset = FVector2D(RandomX, RandomY);

	PlayFadeOutAnimation();
}

void UDamageTextWidget::PlayFadeOutAnimation()
{
	if (FadeOutAnimation)
	{
		PlayAnimation(FadeOutAnimation);
	}
}
