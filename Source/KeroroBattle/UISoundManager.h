// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KeroroBattle.h"
#include "UObject/NoExportTypes.h"
#include "UISoundManager.generated.h"

/**
 *
 */
UCLASS()
class KEROROBATTLE_API UUISoundManager : public UObject
{
	GENERATED_BODY()

public:
	UUISoundManager();

	void PlayUISound(EUISoundType Type);

private:
	USoundBase* HoverSound;
	USoundBase* ClickSound;
};
