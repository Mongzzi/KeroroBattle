// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KeroroBattle.h"
#include "Engine/DataTable.h"
#include "Engine/GameInstance.h"
#include "KeroroGameInstance.generated.h"


USTRUCT(BlueprintType)
struct FKRStatData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FKRStatData();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	int32 Level;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Data)
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float MaxMP;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float AttackPower;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	int32 DropExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	int32 NextExp;

};


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
	FKRStatData* GetKRStatData(int Level);
	
private:
	UPROPERTY()
	UDataTable* KRStatTable;

};
