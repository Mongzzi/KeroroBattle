// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_AttackEnemy.h"
#include "KeroroCharacter.h"
#include "KeroroAIController.h"
#include "KeroroAnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTTask_AttackEnemy::UBTTask_AttackEnemy()
{
	bNotifyTick = true;
	IsAttacking = false;
}

EBTNodeResult::Type UBTTask_AttackEnemy::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto KRCharacter = Cast<AKeroroCharacter>(OwnerComp.GetAIOwner()->GetCharacter());
	if (KRCharacter == nullptr) return EBTNodeResult::Failed;

	// 타겟 방향으로 회전
	AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AKeroroAIController::TargetKey));
	if (Target)
	{
		FRotator LookRot = (Target->GetActorLocation() - KRCharacter->GetActorLocation()).Rotation();
		KRCharacter->SetActorRotation(LookRot);
	}

	KRCharacter->Attack();
	IsAttacking = true;

	KRCharacter->KRAnim->OnMontageEnded.RemoveDynamic(this, &UBTTask_AttackEnemy::OnAttackMontageEnded);
	KRCharacter->KRAnim->OnMontageEnded.AddDynamic(this, &UBTTask_AttackEnemy::OnAttackMontageEnded);


	return EBTNodeResult::InProgress;
}

void UBTTask_AttackEnemy::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsAttacking = false;
}

void UBTTask_AttackEnemy::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
