// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Detect.h"
#include "EnemyAIController.h"
#include "KeroroCharacter.h"
#include "KeroroEnemyCharacter.h"
#include "KeroroAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EngineUtils.h"	// TActorIterator »ç¿ë
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.5f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (AIPawn == nullptr)return;

	auto World = AIPawn->GetWorld();
	if (World == nullptr) return;


	auto TargetObj = OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEnemyAIController::TargetKey);
	if (!IsValid(TargetObj))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(AEnemyAIController::TargetKey, nullptr);
	}

	AKeroroCharacter* NearestKeroro = nullptr;
	float NearestDist = FLT_MAX;
	for (TActorIterator<AKeroroCharacter>IT(World); IT; ++IT)
	{
		AKeroroCharacter* Keroro = *IT;
		if (Keroro == nullptr) continue;

		float dist = FVector::Dist(Keroro->GetActorLocation(), AIPawn->GetActorLocation());
		if (dist < NearestDist)
		{
			NearestDist = dist;
			NearestKeroro = Keroro;
		}
	}
	AKeroroEnemyCharacter* EnemyCharacter = Cast<AKeroroEnemyCharacter>(OwnerComp.GetAIOwner()->GetCharacter());

	if (NearestKeroro)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(AEnemyAIController::TargetKey, NearestKeroro);
		EnemyCharacter->EnemyAnim->bIsRunning = true;
		EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed = EnemyCharacter->MaxWalkSpeed;
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(AEnemyAIController::TargetKey, nullptr);
		EnemyCharacter->EnemyAnim->bIsRunning = false;
		EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed = 200.0f;
	}

}