// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Detect.h"
#include "EnemyAIController.h"
#include "KeroroCharacter.h"
#include "KeroroEnemyCharacter.h"
#include "KeroroAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EngineUtils.h"	// TActorIterator 사용
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (AIPawn == nullptr)return;

	auto World = AIPawn->GetWorld();
	if (World == nullptr) return;

	float DetectRadius = 600.0f;
	float NoDetectRadius = 1200.0f;

	AKeroroCharacter* NearestKeroro = nullptr;
	float NearestDist = FLT_MAX;
	for (TActorIterator<AKeroroCharacter>IT(World); IT; ++IT)
	{
		AKeroroCharacter* Keroro = *IT;
		if (Keroro == nullptr) continue;

		float dist = FVector::Dist(Keroro->GetActorLocation(), AIPawn->GetActorLocation());
		if (dist < DetectRadius && dist < NearestDist)
		{
			NearestDist = dist;
			NearestKeroro = Keroro;
		}
	}
	// NoDetectRadius(1200) 거리이상 멀어지면 타겟키 널로 초기화하여 findpatrol가게설정
	// 셀렉터노드에 데코레이터 is set 설정하여 가능하게함
	AKeroroEnemyCharacter* EnemyCharacter = Cast<AKeroroEnemyCharacter>(OwnerComp.GetAIOwner()->GetCharacter());
	if (NearestDist > NoDetectRadius)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(AEnemyAIController::TargetKey, nullptr);
		EnemyCharacter->EnemyAnim->bIsRunning = false;
		EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	}
	else
	{
		if (NearestKeroro)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(AEnemyAIController::TargetKey, NearestKeroro);
			EnemyCharacter->EnemyAnim->bIsRunning = true;
			EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed = 700.0f;
		}
	}
	//DrawDebugSphere(World, AIPawn->GetActorLocation(), DetectRadius, 16, FColor::Red, false, 0.2f);
	//DrawDebugSphere(World, AIPawn->GetActorLocation(), NoDetectRadius, 16, FColor::Green, false, 0.2f);
}