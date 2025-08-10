// Fill out your copyright notice in the Description page of Project Settings.


#include "KeroroGameMode.h"
#include "KeroroCharacter.h"
#include "KeroroPlayerController.h"
#include "KeroroPlayerState.h"
#include "KeroroGameState.h"
#include "Kismet/GameplayStatics.h"

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
        // 미션 완수 위젯
        // 캐릭터 해금 위젯 추가
    }
    else {
        UE_LOG(LogTemp, Error, TEXT("Mission Failed"));
        // 미션 실패 위젯
    }
}
