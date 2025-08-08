// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTurnToEnemy.generated.h"

/**
 * 
 */
UCLASS()
class KEROROBATTLE_API UBTTurnToEnemy : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTurnToEnemy();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
