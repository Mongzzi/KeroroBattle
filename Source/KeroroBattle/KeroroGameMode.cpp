// Fill out your copyright notice in the Description page of Project Settings.


#include "KeroroGameMode.h"
#include "KeroroCharacter.h"
#include "KeroroPlayerController.h"
#include "KeroroPlayerState.h"
#include "KeroroGameState.h"
#include "Kismet/GameplayStatics.h"
#include "KeroroGameInstance.h"

AKeroroGameMode::AKeroroGameMode()
{
	DefaultPawnClass = AKeroroCharacter::StaticClass();
	PlayerControllerClass = AKeroroPlayerController::StaticClass();
	PlayerStateClass = AKeroroPlayerState::StaticClass();
	GameStateClass = AKeroroGameState::StaticClass();
}

void AKeroroGameMode::BeginPlay()
{
	Super::BeginPlay();
    KeroroGameState = GetGameState<AKeroroGameState>();
    if (KeroroGameState)
    {
        FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);
        if (CurrentLevelName == TEXT("MainLevel1"))
        {
            SurvivalTime = 10.0f;
        }
        else if (CurrentLevelName == TEXT("MainLevel2"))
        {
            SurvivalTime = 240.0f;
        }
        else if (CurrentLevelName == TEXT("MainLevel3"))
        {
            SurvivalTime = 300.0f;
        }

        KeroroGameState->RemainingTime = SurvivalTime;
    }
}

void AKeroroGameMode::OnTimeOver()
{
    EndGame(true);
}

void AKeroroGameMode::EndGame(bool bIsVictory)
{
    if (bIsVictory)
    {
        UE_LOG(LogTemp, Error, TEXT("Mission Complete"));
        if (UKeroroGameInstance* GI = Cast<UKeroroGameInstance>(GetGameInstance()))
        {
            GI->LoadLevelWithLoadingScreen(FName(TEXT("Robby1Level")));
        }
    }
    else {
        UE_LOG(LogTemp, Error, TEXT("Mission Failed"));
        //if (UKeroroGameInstance* GI = Cast<UKeroroGameInstance>(GetGameInstance()))
        //{
        //    GI->LoadLevelWithLoadingScreen(FName(TEXT("Robby1Level")));
        //}
    }
}
