// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KeroroBattle.h"
#include "AIController.h"
#include "KeroroAIController.generated.h"

/**
 * 
 */
UCLASS()
class KEROROBATTLE_API AKeroroAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AKeroroAIController();
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	virtual void BeginPlay() override;

	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;


private:

	UPROPERTY()
	class UBehaviorTree* BTAsset;

	UPROPERTY()
	class UBlackboardData* BBAsset;

	class UBlackboardComponent* BlackboardComponent;
};
