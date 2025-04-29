// Fill out your copyright notice in the Description page of Project Settings.


#include "KeroroAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

AKeroroAIController::AKeroroAIController()
{
	RepeatInterval = 2.0f;
}

void AKeroroAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	GetWorld()->GetTimerManager().SetTimer(RepeatTimerHandle, this, &AKeroroAIController::OnRepeatTimer, RepeatInterval, true);
}

void AKeroroAIController::OnUnPossess()
{
	Super::OnUnPossess();
	GetWorld()->GetTimerManager().ClearTimer(RepeatTimerHandle);
}

void AKeroroAIController::OnRepeatTimer()
{
	auto CurrentPawn = GetPawn();
	if (CurrentPawn == nullptr) return;

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (NavSystem == nullptr) return;

	FNavLocation NextLocation;
	if (NavSystem->GetRandomPointInNavigableRadius(FVector(1530.0f, 1981.0f,90.0f), 1000.0f, NextLocation))
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, NextLocation.Location);
	}
}

