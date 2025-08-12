// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadingGameMode.h"
#include "KRLoadingWidget.h"
#include "KeroroGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/AssetManager.h"

ALoadingGameMode::ALoadingGameMode()
{
	static ConstructorHelpers::FClassFinder<UKRLoadingWidget> WIDGET(TEXT("/Game/Blueprints/LoadingWidget.LoadingWidget_C"));
	if (WIDGET.Succeeded())
	{
		LoadingWidgetClass = WIDGET.Class;
	}
}

void ALoadingGameMode::BeginPlay()
{
	Super::BeginPlay();
	LoadingWidget = CreateWidget<UKRLoadingWidget>(GetWorld(), LoadingWidgetClass);
	if (LoadingWidget)
	{
		LoadingWidget->AddToViewport();
		LoadingWidget->StartLoading();
	}
	StartAsyncLoad();
}

void ALoadingGameMode::StartAsyncLoad()
{
    if (UKeroroGameInstance* GI = Cast<UKeroroGameInstance>(GetGameInstance()))
    {
        FName NextLevelName = GI->NextMapName;
        FName CurrentLevelName = FName(*UGameplayStatics::GetCurrentLevelName(GetWorld(), true));

        if (GI->NextMapName.IsNone() || GI->NextMapName == CurrentLevelName)
        {
            return;
        }

        FString LevelAssetPath = FString::Printf(TEXT("/Game/KeroroLevel/%s.%s"),*GI->NextMapName.ToString(), *GI->NextMapName.ToString());
        FSoftObjectPath LevelPath(LevelAssetPath);

        UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(LevelPath, [this, GI]()
            {
                UGameplayStatics::OpenLevel(GetWorld(), GI->NextMapName);
            });
    }
}

