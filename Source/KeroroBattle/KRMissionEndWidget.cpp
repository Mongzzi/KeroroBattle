// Fill out your copyright notice in the Description page of Project Settings.


#include "KRMissionEndWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "KeroroGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UKRMissionEndWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (RobbyButton)
	{
		RobbyButton->OnClicked.AddDynamic(this, &UKRMissionEndWidget::OnClickedRobbyButton);
	}

	if (ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this, &UKRMissionEndWidget::OnClickedExitButton);
	}
}

void UKRMissionEndWidget::NativeDestruct()
{
	Super::NativeDestruct();
	if (RobbyButton)
	{
		RobbyButton->OnClicked.RemoveDynamic(this, &UKRMissionEndWidget::OnClickedRobbyButton);
	}

	if (ExitButton)
	{
		ExitButton->OnClicked.RemoveDynamic(this, &UKRMissionEndWidget::OnClickedExitButton);
	}
}

void UKRMissionEndWidget::PlayMissionEndAnim()
{
	if (MissionAnim)
	{
		PlayAnimation(MissionAnim);
	}
}

void UKRMissionEndWidget::ChangeDefeatImage()
{
	UTexture2D* Image = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Texture/6.6"));
	if (ResultImage && Image)
	{
		ResultImage->SetBrushFromTexture(Image);
	}
}

void UKRMissionEndWidget::ChangeDefeatText()
{
	if (ResultText)
	{
		ResultText->SetText(FText::FromString(TEXT("미션 실패..")));
	}
}

void UKRMissionEndWidget::OnClickedRobbyButton()
{
	if (UKeroroGameInstance* GI = Cast<UKeroroGameInstance>(GetGameInstance()))
	{
		GI->LoadLevelWithLoadingScreen(TEXT("Robby1Level"));
	}
}

void UKRMissionEndWidget::OnClickedExitButton()
{
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
}
