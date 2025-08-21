// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MoveToIfNotInAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class KEROROBATTLE_API UBTTask_MoveToIfNotInAttackRange : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_MoveToIfNotInAttackRange();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
