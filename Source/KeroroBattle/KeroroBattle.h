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

DECLARE_LOG_CATEGORY_EXTERN(KeroroBattle, Log, All);


#define ABLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))

#define ABLOG_S(Verbosity) UE_LOG(ArenaBattle, Verbosity,TEXT("%s"), *ABLOG_CALLINFO)

#define ABLOG(Verbosity,Format, ...) UE_LOG(ArenaBattle,Verbosity,TEXT("%s%s"),*ABLOG_CALLINFO,*FString::Printf(Format,##__VA_ARGS__))

#define ABCHECK(Expr,...){if(!(Expr)){ABLOG(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__;}}