// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSelectWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "KeroroGameInstance.h"

void UGameSelectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	KRGI = Cast<UKeroroGameInstance>(GetGameInstance());
	if (!KRGI) return;

	if (SelectButton1)
	{
		SelectButton1->OnClicked.AddDynamic(this, &UGameSelectWidget::OnSelectButton1Clicked);
		SelectButton1->OnHovered.AddDynamic(this, &UGameSelectWidget::OnButtonHoverd);
	}
	if (SelectButton2)
	{
		SelectButton2->OnClicked.AddDynamic(this, &UGameSelectWidget::OnSelectButton2Clicked);
		SelectButton2->OnHovered.AddDynamic(this, &UGameSelectWidget::OnButtonHoverd);
	}
	if (SelectButton3)
	{
		SelectButton3->OnClicked.AddDynamic(this, &UGameSelectWidget::OnSelectButton3Clicked);
		SelectButton3->OnHovered.AddDynamic(this, &UGameSelectWidget::OnButtonHoverd);
	}
	if (SelectButton4)
	{
		SelectButton4->OnClicked.AddDynamic(this, &UGameSelectWidget::OnSelectButton4Clicked);
		SelectButton4->OnHovered.AddDynamic(this, &UGameSelectWidget::OnButtonHoverd);
	}
	if (SelectButton5)
	{
		SelectButton5->OnClicked.AddDynamic(this, &UGameSelectWidget::OnSelectButton5Clicked);
		SelectButton5->OnHovered.AddDynamic(this, &UGameSelectWidget::OnButtonHoverd);
	}
}

void UGameSelectWidget::NativeDestruct()
{
	Super::NativeDestruct();
	if (SelectButton1)
	{
		SelectButton1->OnClicked.RemoveDynamic(this, &UGameSelectWidget::OnSelectButton1Clicked);
		SelectButton1->OnHovered.RemoveDynamic(this, &UGameSelectWidget::OnButtonHoverd);
	}
	if (SelectButton2)
	{
		SelectButton2->OnClicked.RemoveDynamic(this, &UGameSelectWidget::OnSelectButton2Clicked);
		SelectButton2->OnHovered.RemoveDynamic(this, &UGameSelectWidget::OnButtonHoverd);
	}
	if (SelectButton3)
	{
		SelectButton3->OnClicked.RemoveDynamic(this, &UGameSelectWidget::OnSelectButton3Clicked);
		SelectButton3->OnHovered.RemoveDynamic(this, &UGameSelectWidget::OnButtonHoverd);
	}
	if (SelectButton4)
	{
		SelectButton4->OnClicked.RemoveDynamic(this, &UGameSelectWidget::OnSelectButton4Clicked);
		SelectButton4->OnHovered.RemoveDynamic(this, &UGameSelectWidget::OnButtonHoverd);
	}
	if (SelectButton5)
	{
		SelectButton5->OnClicked.RemoveDynamic(this, &UGameSelectWidget::OnSelectButton5Clicked);
		SelectButton5->OnHovered.RemoveDynamic(this, &UGameSelectWidget::OnButtonHoverd);
	}
}

void UGameSelectWidget::OnButtonHoverd()
{
	KRGI->PlayUISound(EUISoundType::Hover);
}

void UGameSelectWidget::OnSelectButton1Clicked()
{
	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		PC->bShowMouseCursor = false;
		PC->SetInputMode(FInputModeGameOnly());
	}
	//UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("MainLevel1")));
	if (UKeroroGameInstance* GI = Cast<UKeroroGameInstance>(GetGameInstance()))
	{
		GI->NextMissionRound = EKeroroType::Tamama;
		GI->LoadLevelWithLoadingScreen(FName(TEXT("MainLevel1")));
	}
}

void UGameSelectWidget::OnSelectButton2Clicked()
{
	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		PC->bShowMouseCursor = false;
		PC->SetInputMode(FInputModeGameOnly());
	}
	//UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("MainLevel1")));
	if (UKeroroGameInstance* GI = Cast<UKeroroGameInstance>(GetGameInstance()))
	{
		GI->NextMissionRound = EKeroroType::Giroro;
		GI->LoadLevelWithLoadingScreen(FName(TEXT("MainLevel1")));
	}
}

void UGameSelectWidget::OnSelectButton3Clicked()
{
	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		PC->bShowMouseCursor = false;
		PC->SetInputMode(FInputModeGameOnly());
	}
	//UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("MainLevel3")));
	if (UKeroroGameInstance* GI = Cast<UKeroroGameInstance>(GetGameInstance()))
	{
		GI->NextMissionRound = EKeroroType::Dororo;
		GI->LoadLevelWithLoadingScreen(FName(TEXT("MainLevel3")));
	}
}

void UGameSelectWidget::OnSelectButton4Clicked()
{
	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		PC->bShowMouseCursor = false;
		PC->SetInputMode(FInputModeGameOnly());
	}
	if (UKeroroGameInstance* GI = Cast<UKeroroGameInstance>(GetGameInstance()))
	{
		GI->NextMissionRound = EKeroroType::Kururu;
		GI->LoadLevelWithLoadingScreen(FName(TEXT("MainLevel2")));
	}
}

void UGameSelectWidget::OnSelectButton5Clicked()
{
	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		PC->bShowMouseCursor = false;
		PC->SetInputMode(FInputModeGameOnly());
	}
	if (UKeroroGameInstance* GI = Cast<UKeroroGameInstance>(GetGameInstance()))
	{
		GI->NextMissionRound = EKeroroType::Keroro;
		GI->LoadLevelWithLoadingScreen(FName(TEXT("MainLevel3")));
	}
}
