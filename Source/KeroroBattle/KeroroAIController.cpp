// Fill out your copyright notice in the Description page of Project Settings.


#include "KeroroAIController.h"
#include "KeroroEnemyCharacter.h"
#include "KeroroPlayerState.h"
#include "KeroroStatComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName AKeroroAIController::HomePosKey(TEXT("HomePos"));
const FName AKeroroAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName AKeroroAIController::TargetKey(TEXT("TargetEnemy"));

AKeroroAIController::AKeroroAIController()
{
		BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("KRBBCOMP"));
	
		static ConstructorHelpers::FObjectFinder<UBlackboardData>BLACKBOARD(TEXT("/Game/AI/BB_Keroro.BB_Keroro"));
		if (BLACKBOARD.Succeeded())
		{
			BBAsset = BLACKBOARD.Object;
		}
	
		static ConstructorHelpers::FObjectFinder<UBehaviorTree>BEHAVIORTREE(TEXT("/Game/AI/BT_Keroro.BT_Keroro"));
		if (BEHAVIORTREE.Succeeded())
		{
			BTAsset = BEHAVIORTREE.Object;
		}
}

void AKeroroAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	BlackboardComponent = GetBlackboardComponent();
	if (UseBlackboard(BBAsset, BlackboardComponent))
	{
		Blackboard->SetValueAsVector(AKeroroAIController::HomePosKey, InPawn->GetActorLocation());
		RunBehaviorTree(BTAsset);
	}
}

void AKeroroAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void AKeroroAIController::BeginPlay()
{
	Super::BeginPlay();
}



