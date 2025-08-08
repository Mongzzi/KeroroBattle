// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnToKeororo.h"
#include "EnemyAIController.h"
#include "KeroroEnemyCharacter.h"
#include "KeroroCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"


UTurnToKeororo::UTurnToKeororo()
{
	NodeName = TEXT("Turn To Keroro");
}

EBTNodeResult::Type UTurnToKeororo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Charater = Cast<AKeroroEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (Charater == nullptr) return EBTNodeResult::Failed;

	auto Target = Cast<AKeroroCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEnemyAIController::TargetKey));
	if (Target == nullptr) return EBTNodeResult::Failed;

	FVector Direction = Target->GetActorLocation() - Charater->GetActorLocation();
	Direction.Z = 0.0f;

	FRotator TargetRotation = Direction.Rotation();
	FRotator NewRotation = FMath::RInterpTo(Charater->GetActorRotation(), TargetRotation, GetWorld()->GetDeltaSeconds(), 2.0f);
	Charater->SetActorRotation(NewRotation);

	return EBTNodeResult::Succeeded;
}
