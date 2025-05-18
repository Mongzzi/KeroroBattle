// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


UENUM(BlueprintType)
enum class EKeroroType : uint8
{
    Keroro     UMETA(DisplayName = "Keroro"),
    Tamama     UMETA(DisplayName = "Tamama"),
    Giroro     UMETA(DisplayName = "Giroro"),
    Kururu     UMETA(DisplayName = "Kururu"),
    Dororo     UMETA(DisplayName = "Dororo"),
    MAX UMETA(Hidden) // 순환 계산용
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    KEROBALL     UMETA(DisplayName = "KEROBALL"),
    RIFLE   UMETA(DisplayName = "RIFLE"),
    SWORD   UMETA(DisplayName = "SWORD"),
    FIST   UMETA(DisplayName = "FIST"),
    MAX     UMETA(Hidden) // 순환 계산용
};