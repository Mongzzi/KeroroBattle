// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "EnemyAIController.h"
#include "KeroroEnemyCharacter.h"
#include "KeroroCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;
	IsAttacking = false;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto EnemyCharacter = Cast<AKeroroEnemyCharacter>(OwnerComp.GetAIOwner()->GetCharacter());
	if (EnemyCharacter == nullptr) return EBTNodeResult::Failed;

	auto Target = Cast<AKeroroCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEnemyAIController::TargetKey));
	if (Target == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	float dist = FVector::Dist(Target->GetActorLocation(), EnemyCharacter->GetActorLocation());
	
	if (dist <= EnemyCharacter->CanComboAttackDist)
	{
		EnemyCharacter->Attack();
		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	auto EnemyCharacter = Cast<AKeroroEnemyCharacter>(OwnerComp.GetAIOwner()->GetCharacter());
	if (!EnemyCharacter) return;

	// 애니메이션 끝나고 다음 콤보 가능한 경우 자동 콤보 연결
	if (EnemyCharacter->bCanNextCombo)
	{
		EnemyCharacter->Attack(); // 다음 콤보로 연결
	}

	// 콤보 끝났다면 태스크 종료
	if (!EnemyCharacter->bIsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
