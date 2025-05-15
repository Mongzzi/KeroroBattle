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
    EMPTY   UMETA(DisplayName = "EMPTY"),
    TNT     UMETA(DisplayName = "TNT"),
    RIFLE   UMETA(DisplayName = "RIFLE"),
    SWORD   UMETA(DisplayName = "SWORD"),
    MAX     UMETA(Hidden) // 순환 계산용
};