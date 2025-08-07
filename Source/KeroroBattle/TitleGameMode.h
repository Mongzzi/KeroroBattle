// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TitleGameMode.generated.h"

/**
 * 
 */
UCLASS()
class KEROROBATTLE_API ATitleGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATitleGameMode();

	virtual void BeginPlay() override;

	UPROPERTY()
	TSubclassOf<class UTitleWidget> TitleWidgetClass;

	UPROPERTY()
	class UTitleWidget* TitleWidget;
};
