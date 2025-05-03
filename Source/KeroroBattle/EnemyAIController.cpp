// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName AEnemyAIController::HomePosKey(TEXT("HomePos"));
const FName AEnemyAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName AEnemyAIController::TargetKey(TEXT("TargetKeroro"));

AEnemyAIController::AEnemyAIController()
{
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BBCOMPONENT"));

	static ConstructorHelpers::FObjectFinder<UBlackboardData>BLACKBOARD(TEXT("/Game/AI/BB_Enemy.BB_Enemy"));
	if (BLACKBOARD.Succeeded())
	{
		BBAsset = BLACKBOARD.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree>BEHAVIORTREE(TEXT("/Game/AI/BT_Enemy.BT_Enemy"));
	if (BEHAVIORTREE.Succeeded())
	{
		BTAsset = BEHAVIORTREE.Object;
	}
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	BlackboardComponent = GetBlackboardComponent();
	if (UseBlackboard(BBAsset, BlackboardComponent))
	{

		Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
		RunBehaviorTree(BTAsset);
	}
}

void AEnemyAIController::OnUnPossess()
{
	Super::OnUnPossess();
}



