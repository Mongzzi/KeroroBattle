// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTurnToEnemy.h"
#include "KeroroAIController.h"
#include "KeroroEnemyCharacter.h"
#include "KeroroCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTurnToEnemy::UBTTurnToEnemy()
{
	NodeName = TEXT("Turn To Enemy");
}

EBTNodeResult::Type UBTTurnToEnemy::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Character = Cast<AKeroroCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (Character == nullptr) return EBTNodeResult::Failed;

	auto Target = Cast<AKeroroEnemyCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AKeroroAIController::TargetKey));
	if (Target == nullptr) return EBTNodeResult::Failed;

	FVector Direction = Target->GetActorLocation() - Character->GetActorLocation();
	Direction.Z = 0.0f;

	FRotator TargetRotation = Direction.Rotation();
	FRotator NewRotation = FMath::RInterpTo(Character->GetActorRotation(), TargetRotation, GetWorld()->GetDeltaSeconds(), 2.0f);
	Character->SetActorRotation(NewRotation);

	return EBTNodeResult::Succeeded;
}
