// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KeroroBattle.h"
#include "Engine/DataTable.h"
#include "Engine/GameInstance.h"
#include "Engine/StreamableManager.h"
#include "KeroroGameInstance.generated.h"



USTRUCT(BlueprintType)
struct FKRFaceAssetData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FKRFaceAssetData();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Asset)
	EKeroroType KeroroType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Asset)
	EFaceType FaceType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Asset)
	FSoftObjectPath AssetPath;

};

USTRUCT(BlueprintType)
struct FKRSoundAssetData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FKRSoundAssetData();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Asset)
	EKeroroType KeroroType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Asset)
	ESoundType SoundType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Asset)
	FSoftObjectPath AssetPath;
};

USTRUCT(BlueprintType)
struct FKRStatData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FKRStatData();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float MaxMP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float AttackPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	int32 DropExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	int32 NextExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float MaxMoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float CritChanceRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float AttackSpeedRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float SkillCooldownRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float EvasionRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float DefenseRate;
};

USTRUCT(BlueprintType)
struct FCardData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FCardData();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardData")
	int32 CardID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardData")
	FText CardName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardData")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardData")
	FName CardType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardData")
	float CardValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardData")
	FSoftObjectPath ImagePath;
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
	FCardData* GetCardData(int CardID);
	FSoftObjectPath GetFaceAssetPath(EKeroroType KeroroType, EFaceType FaceType);
	FSoftObjectPath GetVoiceSoundAssetPath(EKeroroType KeroroType, int32 index);
	FStreamableManager& GetStreamableManager();

private:
	FStreamableManager StreamableManager;

private:
	UPROPERTY()
	UDataTable* KRStatTable;

	UPROPERTY()
	UDataTable* CardTable;

	UPROPERTY()
	UDataTable* FaceAssetTable;

	UPROPERTY()
	UDataTable* KRSoundAssetTable;
};
