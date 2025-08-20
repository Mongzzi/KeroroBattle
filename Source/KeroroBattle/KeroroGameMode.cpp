// Fill out your copyright notice in the Description page of Project Settings.


#include "KeroroGameMode.h"
#include "KeroroCharacter.h"
#include "KeroroPlayerController.h"
#include "KeroroPlayerState.h"
#include "KeroroGameState.h"
#include "Kismet/GameplayStatics.h"
#include "KeroroGameInstance.h"
#include "KRMissionEndWidget.h"
#include "KRUnlockWidget.h"
#include "GameFramework/PlayerStart.h"

AKeroroGameMode::AKeroroGameMode()
{
	DefaultPawnClass = AKeroroCharacter::StaticClass();
	PlayerControllerClass = AKeroroPlayerController::StaticClass();
	PlayerStateClass = AKeroroPlayerState::StaticClass();
	GameStateClass = AKeroroGameState::StaticClass();

	static ConstructorHelpers::FClassFinder<UKRMissionEndWidget>WIDGET1(TEXT("/Game/Blueprints/MissionEndWidget.MissionEndWidget_C"));
	if (WIDGET1.Succeeded())
	{
		MissionWidgetClass = WIDGET1.Class;
	}
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
			SurvivalTime = 160.0f;
		}
		else if (CurrentLevelName == TEXT("MainLevel3"))
		{
			SurvivalTime = 160.0f;
		}
		else
		{
			KeroroGameState->bIsMainMap = false;
			return;
		}
		KeroroGameState->RemainingTime = SurvivalTime;
		KeroroGameState->bIsMainMap = true;
	}
}

AActor* AKeroroGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	UKeroroGameInstance* GI = GetGameInstance<UKeroroGameInstance>();
	FString TargetStartName;

	if (GI)
	{
		//보스전일때 playerstart2지점에서 시작
		if (GI->NextMissionRound == EKeroroType::Keroro)
		{
			TargetStartName = TEXT("PlayerStart2");
		}
	}

	TArray<AActor*> FoundStarts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), FoundStarts);

	for (AActor* Start : FoundStarts)
	{
		if (Start->GetName().Contains(TargetStartName))
		{
			return Start;
		}
	}

	// 못찾으면 기존대로
	return Super::ChoosePlayerStart_Implementation(Player);
}

void AKeroroGameMode::OnTimeOver()
{
	EndGame(true);
}

void AKeroroGameMode::EndGame(bool bIsVictory)
{
	AKeroroPlayerController* PC = Cast<AKeroroPlayerController>(GetWorld()->GetFirstPlayerController());
	if (!PC) return;

	AKeroroPlayerState* PS = Cast<AKeroroPlayerState>(PC->PlayerState);
	if (!PS) return;

	UKeroroGameInstance* GI = Cast<UKeroroGameInstance>(GetGameInstance());
	if (!GI) return;

	if (bIsVictory)
	{
		MissionEndWidget = CreateWidget<UKRMissionEndWidget>(GetWorld(), MissionWidgetClass);
		if (MissionEndWidget)
		{
			MissionEndWidget->AddToViewport();
			MissionEndWidget->PlayMissionEndAnim();
			EKeroroType MissionRound = GI->NextMissionRound;

			// 언락 위젯
			if (!GI->IsCharacterUnlocked(MissionRound))
			{
				GI->UnlockCharacter(MissionRound);
				MissionEndWidget->PlayUnlockAnim();
				MissionEndWidget->UnlockWidget->ChangeImage(MissionRound);
				MissionEndWidget->UnlockWidget->ChangeText(MissionRound);

				FTimerHandle RemoveHandle;
				TWeakObjectPtr<UKRMissionEndWidget> WeakMissionEndWidget = MissionEndWidget;
				GetWorldTimerManager().SetTimer(RemoveHandle, [WeakMissionEndWidget]()
					{
						if (WeakMissionEndWidget.IsValid() && WeakMissionEndWidget->UnlockWidget)
						{
							WeakMissionEndWidget->UnlockWidget->RemoveFromParent();
							WeakMissionEndWidget->UnlockWidget = nullptr;
						}
					}, 3.0f, false);
			}
			PC->SetUIMode();
		}
	}
	else {
		MissionEndWidget = CreateWidget<UKRMissionEndWidget>(GetWorld(), MissionWidgetClass);
		if (MissionEndWidget)
		{
			MissionEndWidget->AddToViewport();
			MissionEndWidget->ChangeDefeatImage();
			MissionEndWidget->ChangeDefeatText();
			MissionEndWidget->PlayMissionEndAnim();
			PC->SetUIMode();
		}
	}
}
