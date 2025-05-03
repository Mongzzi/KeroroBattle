// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsInAttackRangeEnemy.h"
#include "KeroroAIController.h"
#include "KeroroEnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsInAttackRangeEnemy::UBTDecorator_IsInAttackRangeEnemy()
{
	NodeName = TEXT("CanAttackKR");
}

bool UBTDecorator_IsInAttackRangeEnemy::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr) return false;

	auto Target = Cast<AKeroroEnemyCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AKeroroAIController::TargetKey));
	if (Target == nullptr) return false;

	bResult = (Target->GetDistanceTo(ControllingPawn) <= 200.0f);
	return bResult;
}
