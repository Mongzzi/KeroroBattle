// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsInAttackRange.h"
#include "EnemyAIController.h"
#include "KeroroCharacter.h"
#include "KeroroEnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{
	NodeName = TEXT("CanAttack");
}

// 데코레이터 조건 참거짓 반환 함수
bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	AKeroroEnemyCharacter* OwnerEnemyKero = Cast<AKeroroEnemyCharacter>(OwnerComp.GetAIOwner()->GetCharacter());
	if (OwnerEnemyKero == nullptr) return false;

	auto Target = Cast<AKeroroCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEnemyAIController::TargetKey));
	if (Target == nullptr) return false;

	bResult = (OwnerEnemyKero->GetDistanceTo(Target) <= OwnerEnemyKero->AttackRange);
	return bResult;
}
