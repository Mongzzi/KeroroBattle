// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsInAttackRangeEnemy.generated.h"

/**
 * 
 */
UCLASS()
class KEROROBATTLE_API UBTDecorator_IsInAttackRangeEnemy : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_IsInAttackRangeEnemy();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
