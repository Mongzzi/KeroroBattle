// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KeroroBattle.h"
#include "Engine/GameInstance.h"
#include "KeroroGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class KEROROBATTLE_API UKeroroGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UKeroroGameInstance();

	virtual void Init() override;
	
};
