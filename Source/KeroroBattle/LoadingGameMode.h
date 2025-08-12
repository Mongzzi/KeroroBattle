// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LoadingGameMode.generated.h"

/**
 * 
 */
UCLASS()
class KEROROBATTLE_API ALoadingGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALoadingGameMode();

protected:
	virtual void BeginPlay() override;

	void StartAsyncLoad();

	UPROPERTY()
	class UKRLoadingWidget* LoadingWidget;

	UPROPERTY()
	TSubclassOf<class UKRLoadingWidget> LoadingWidgetClass;
};
