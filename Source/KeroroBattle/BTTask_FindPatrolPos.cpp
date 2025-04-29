// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPatrolPos.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
	NodeName=TEXT("FindPatrolPos");
}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (AIPawn == nullptr) return EBTNodeResult::Failed;

	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetNavigationSystem(AIPawn->GetWorld());
	if (NavSys == nullptr) return EBTNodeResult::Failed;

	FNavLocation PatrolLocation;

	// 500.0f 범위 내에서 랜덤 위치 찾기
	if (NavSys->GetRandomReachablePointInRadius(AIPawn->GetActorLocation(), 1500.0f, PatrolLocation))
	{
		// Blackboard에 저장
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName("PatrolPos"), PatrolLocation.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
