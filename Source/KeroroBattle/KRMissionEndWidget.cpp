// Fill out your copyright notice in the Description page of Project Settings.


#include "KRMissionEndWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "KeroroGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "KRUnlockWidget.h"

void UKRMissionEndWidget::NativeConstruct()
{
	Super::NativeConstruct();
	KRGI = Cast<UKeroroGameInstance>(GetGameInstance());
	if (!KRGI) return;

	if (RobbyButton)
	{
		RobbyButton->OnClicked.AddDynamic(this, &UKRMissionEndWidget::OnClickedRobbyButton);
		RobbyButton->OnHovered.AddDynamic(this, &UKRMissionEndWidget::OnButtonHoverd);
	}

	if (ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this, &UKRMissionEndWidget::OnClickedExitButton);
		ExitButton->OnHovered.AddDynamic(this, &UKRMissionEndWidget::OnButtonHoverd);
	}
}

void UKRMissionEndWidget::NativeDestruct()
{
	Super::NativeDestruct();
	if (RobbyButton)
	{
		RobbyButton->OnClicked.RemoveDynamic(this, &UKRMissionEndWidget::OnClickedRobbyButton);
		RobbyButton->OnHovered.RemoveDynamic(this, &UKRMissionEndWidget::OnButtonHoverd);
	}

	if (ExitButton)
	{
		ExitButton->OnClicked.RemoveDynamic(this, &UKRMissionEndWidget::OnClickedExitButton);
		ExitButton->OnHovered.RemoveDynamic(this, &UKRMissionEndWidget::OnButtonHoverd);
	}
}

void UKRMissionEndWidget::OnButtonHoverd()
{
	KRGI->PlayUISound(EUISoundType::Hover);
}

void UKRMissionEndWidget::PlayMissionEndAnim()
{
	if (MissionAnim)
	{
		PlayAnimation(MissionAnim);
	}
}

void UKRMissionEndWidget::PlayUnlockAnim()
{
	if (UnlockAnim)
	{
		PlayAnimation(UnlockAnim);
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
