// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MoveToIfNotInAttackRange.h"
#include "EnemyAIController.h"
#include "KeroroEnemyCharacter.h"
#include "KeroroCharacter.h"
#include "KeroroAnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"

UBTTask_MoveToIfNotInAttackRange::UBTTask_MoveToIfNotInAttackRange()
{
	NodeName = TEXT("MoveToIfNotInAttackRange");
	bNotifyTick = false;
}

EBTNodeResult::Type UBTTask_MoveToIfNotInAttackRange::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyAIController* AIC = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if (!AIC) return EBTNodeResult::Failed;

	AKeroroEnemyCharacter* EnemyKero = Cast<AKeroroEnemyCharacter>(AIC->GetPawn());
	if (!EnemyKero) return EBTNodeResult::Failed;

	AKeroroCharacter* TargetKero = Cast<AKeroroCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AIC->TargetKey));
	if (!TargetKero) return EBTNodeResult::Failed;

	float Dist = FVector::Dist(TargetKero->GetActorLocation(), EnemyKero->GetActorLocation());
	float AttackRange = EnemyKero->AttackRange;

	if (Dist < AttackRange)
	{
		AIC->StopMovement();
		EnemyKero->EnemyAnim->bIsRunning = false;
		EnemyKero->GetCharacterMovement()->MaxWalkSpeed = 200.f;
		return EBTNodeResult::Succeeded;
	}
	else
	{
		FAIMoveRequest MoveReq;
		MoveReq.SetGoalActor(TargetKero);
		MoveReq.SetAcceptanceRadius(AttackRange/2);

		FNavPathSharedPtr NavPath;
		AIC->MoveTo(MoveReq, &NavPath);

		EnemyKero->EnemyAnim->bIsRunning = true;
		EnemyKero->GetCharacterMovement()->MaxWalkSpeed = 350.f;
		return EBTNodeResult::Succeeded;
	}
}
