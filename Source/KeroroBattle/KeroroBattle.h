// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


UENUM(BlueprintType)
enum class EKeroroType : uint8
{
	Keroro      UMETA(DisplayName = "Keroro"),
	Tamama      UMETA(DisplayName = "Tamama"),
	Giroro      UMETA(DisplayName = "Giroro"),
	Kururu      UMETA(DisplayName = "Kururu"),
	Dororo      UMETA(DisplayName = "Dororo"),
	MAX         UMETA(Hidden) // 순환 계산용
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	KEROBALL     UMETA(DisplayName = "KEROBALL"),
	RIFLE		 UMETA(DisplayName = "RIFLE"),
	SWORD		 UMETA(DisplayName = "SWORD"),
	FIST		 UMETA(DisplayName = "FIST"),
	NOTEBOOK	 UMETA(DisplayName = "NOTEBOOK"),
	MAX			 UMETA(Hidden) // 순환 계산용
};

UENUM(BlueprintType)
enum class EFaceType : uint8
{
	Default			UMETA(DisplayName = "Default"),
	Sad				UMETA(DisplayName = "Sad"),
	Happy			UMETA(DisplayName = "Happy"),
	Fatal			UMETA(DisplayName = "Fatal"),
	Super			UMETA(DisplayName = "Super"),
	Anger			UMETA(DisplayName = "Anger"),
	MAX				UMETA(Hidden) // 순환 계산용
};

UENUM(BlueprintType)
enum class ESoundType : uint8
{
	ComboAttack				UMETA(DisplayName = "ComboAttack"),
	Die						UMETA(DisplayName = "Die"),
	MAX						UMETA(Hidden) // 순환 계산용
};