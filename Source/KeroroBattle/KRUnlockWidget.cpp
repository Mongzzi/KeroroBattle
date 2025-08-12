// Fill out your copyright notice in the Description page of Project Settings.


#include "KRUnlockWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"


void UKRUnlockWidget::ChangeImage(EKeroroType type)
{
    FString ImagePath;

    switch (type)
    {
    case EKeroroType::Tamama:
        ImagePath = TEXT("/Game/Texture/29.29");
        break;
    case EKeroroType::Giroro:
        ImagePath = TEXT("/Game/Texture/25.25");
        break;
    case EKeroroType::Kururu:
        ImagePath = TEXT("/Game/Texture/38.38");
        break;
    case EKeroroType::Dororo:
        ImagePath = TEXT("/Game/Texture/34.34");
        break;
    }

    UTexture2D* Image = LoadObject<UTexture2D>(nullptr, *ImagePath);
    if (UnlockKeroImage && Image)
    {
        UnlockKeroImage->SetBrushFromTexture(Image);
    }
}

void UKRUnlockWidget::ChangeText(EKeroroType type)
{
    if (!UnlockKeroText) return;

    FText NewText;

    switch (type)
    {
    case EKeroroType::Tamama:
        NewText = FText::FromString(TEXT("타마마 캐릭터 해금!"));
        break;
    case EKeroroType::Giroro:
        NewText = FText::FromString(TEXT("기로로 캐릭터 해금!"));
        break;
    case EKeroroType::Kururu:
        NewText = FText::FromString(TEXT("쿠루루 캐릭터 해금!"));
        break;
    case EKeroroType::Dororo:
        NewText = FText::FromString(TEXT("도로로 캐릭터 해금!"));
        break;
    }

    UnlockKeroText->SetText(NewText);
}