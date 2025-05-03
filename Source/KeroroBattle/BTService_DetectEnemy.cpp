// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_DetectEnemy.h"
#include "KeroroAIController.h"
#include "KeroroEnemyCharacter.h"
#include "KeroroCharacter.h"
#include "EngineUtils.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_DetectEnemy::UBTService_DetectEnemy()
{
	NodeName = TEXT("KRDetect");
	Interval = 1.0f;
}

// 아래 코드 템플릿 함수로 빼서 관리 하는게 좋을듯 ( 몬스터와 동일하게 동작 )

void UBTService_DetectEnemy::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (AIPawn == nullptr)return;

	auto World = AIPawn->GetWorld();
	if (World == nullptr) return;

	float DetectRadius = 600.0f;
	float NoDetectRadius = 1200.0f;

	AKeroroEnemyCharacter* NearestEnemy = nullptr;
	float NearestDist = FLT_MAX;

	for (TActorIterator<AKeroroEnemyCharacter>IT(World); IT; ++IT)
	{
		AKeroroEnemyCharacter* Enemy = *IT;
		if (Enemy == nullptr) continue;

		float dist = FVector::Dist(Enemy->GetActorLocation(), AIPawn->GetActorLocation());
		if (dist < DetectRadius && dist < NearestDist)
		{
			NearestDist = dist;
			NearestEnemy = Enemy;
		}
	}

	AKeroroCharacter* Keroro = Cast<AKeroroCharacter>(OwnerComp.GetAIOwner()->GetCharacter());
	if (NearestDist > NoDetectRadius)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(AKeroroAIController::TargetKey, nullptr);
		Keroro->StopRun();
	}
	else
	{
		if (NearestEnemy)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(AKeroroAIController::TargetKey, NearestEnemy);
			Keroro->StartRun();
		}
	}

}
