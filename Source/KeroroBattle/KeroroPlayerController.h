// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "KeroroPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class KEROROBATTLE_API AKeroroPlayerController : public APlayerController
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
};
