// Fill out your copyright notice in the Description page of Project Settings.


#include "UISoundManager.h"
#include "Kismet/GameplayStatics.h"


UUISoundManager::UUISoundManager()
{
	static ConstructorHelpers::FObjectFinder<USoundBase> HOVER(TEXT("/Game/Keroro_Sound/etc/ui7_hover.ui7_hover"));
	if (HOVER.Succeeded()) HoverSound = HOVER.Object;

	static ConstructorHelpers::FObjectFinder<USoundBase> CLICK(TEXT("/Game/Keroro_Sound/etc/ui3_click.ui3_click"));
	if (CLICK.Succeeded()) ClickSound = CLICK.Object;

}

void UUISoundManager::PlayUISound(EUISoundType Type)
{
	USoundBase* PlaySound = nullptr;

	switch (Type)
	{
	case EUISoundType::Hover:
		PlaySound = HoverSound;
		break;
	case EUISoundType::Click:
		PlaySound = ClickSound;
		break;
	}

	if (PlaySound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), PlaySound);
	}
}
