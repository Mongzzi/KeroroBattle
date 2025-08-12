// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSelectWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "KeroroGameInstance.h"

void UGameSelectWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (SelectButton1)
	{
		SelectButton1->OnClicked.AddDynamic(this, &UGameSelectWidget::OnSelectButton1Clicked);
	}
	if (SelectButton2)
	{
		SelectButton2->OnClicked.AddDynamic(this, &UGameSelectWidget::OnSelectButton2Clicked);
	}
	if (SelectButton3)
	{
		SelectButton3->OnClicked.AddDynamic(this, &UGameSelectWidget::OnSelectButton3Clicked);
	}
	if (SelectButton4)
	{
		SelectButton4->OnClicked.AddDynamic(this, &UGameSelectWidget::OnSelectButton4Clicked);
	}
	if (SelectButton5)
	{
		SelectButton5->OnClicked.AddDynamic(this, &UGameSelectWidget::OnSelectButton5Clicked);
	}
}

void UGameSelectWidget::NativeDestruct()
{
	Super::NativeDestruct();
	if (SelectButton1)
	{
		SelectButton1->OnClicked.RemoveDynamic(this, &UGameSelectWidget::OnSelectButton1Clicked);
	}
	if (SelectButton2)
	{
		SelectButton2->OnClicked.RemoveDynamic(this, &UGameSelectWidget::OnSelectButton2Clicked);
	}
	if (SelectButton3)
	{
		SelectButton3->OnClicked.RemoveDynamic(this, &UGameSelectWidget::OnSelectButton3Clicked);
	}
	if (SelectButton4)
	{
		SelectButton4->OnClicked.RemoveDynamic(this, &UGameSelectWidget::OnSelectButton4Clicked);
	}
	if (SelectButton5)
	{
		SelectButton5->OnClicked.RemoveDynamic(this, &UGameSelectWidget::OnSelectButton5Clicked);
	}
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
		GI->LoadLevelWithLoadingScreen(FName(TEXT("MainLevel3")));
	}
}
