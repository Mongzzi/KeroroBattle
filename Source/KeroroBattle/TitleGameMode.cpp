// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleGameMode.h"
#include "TitleWidget.h"

ATitleGameMode::ATitleGameMode()
{
	static ConstructorHelpers::FClassFinder<UTitleWidget>TITLE(TEXT("/Game/Blueprints/Title_Widget.Title_Widget_C"));
	if (TITLE.Succeeded())
	{
		TitleWidgetClass = TITLE.Class;
	}

}

void ATitleGameMode::BeginPlay()
{
    if (TitleWidgetClass)
    {
        TitleWidget = CreateWidget<UTitleWidget>(GetWorld(), TitleWidgetClass);
        if (TitleWidget)
        {
            TitleWidget->AddToViewport();
            TitleWidget->PlayTitleMedia();
            
            APlayerController* PC = GetWorld()->GetFirstPlayerController();
            if (PC)
            {
                PC->bShowMouseCursor = true;
                PC->SetInputMode(FInputModeUIOnly().SetWidgetToFocus(TitleWidget->TakeWidget()));
            }
        }
    }
}
