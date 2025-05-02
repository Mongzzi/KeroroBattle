// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "EnemyAIController.h"
#include "KeroroEnemyCharacter.h"
#include "KeroroCharacter.h"
#include "KeroroAnimInstance.h"
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
	if (Target == nullptr) return EBTNodeResult::Failed;


	float dist = FVector::Dist(Target->GetActorLocation(), EnemyCharacter->GetActorLocation());
	if (dist <= EnemyCharacter->AttackRange)
	{
		IsAttacking = true;
		EnemyCharacter->EnemyAnim->OnMontageEnded.RemoveDynamic(this, &UBTTask_Attack::OnAttackMontageEnded);
		EnemyCharacter->EnemyAnim->OnMontageEnded.AddDynamic(this, &UBTTask_Attack::OnAttackMontageEnded);
		EnemyCharacter->Attack();
		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}


void UBTTask_Attack::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsAttacking = false;
}