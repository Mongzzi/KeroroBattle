// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TurnToKeororo.generated.h"

/**
 * 
 */
UCLASS()
class KEROROBATTLE_API UTurnToKeororo : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UTurnToKeororo();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
