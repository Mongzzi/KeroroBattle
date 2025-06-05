// Fill out your copyright notice in the Description page of Project Settings.


#include "KeroroGameInstance.h"
#include "Engine/StreamableManager.h"

UKeroroGameInstance::UKeroroGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable>KRDT(TEXT("/Game/GameData/StatData.StatData"));
	if (KRDT.Succeeded())KRStatTable = KRDT.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable>CDDT(TEXT("/Game/GameData/CardData.CardData"));
	if (CDDT.Succeeded())CardTable = CDDT.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable>FADT(TEXT("/Game/GameData/FaceAssetData.FaceAssetData"));
	if (FADT.Succeeded()) FaceAssetTable = FADT.Object;

}

void UKeroroGameInstance::Init()
{
	Super::Init();

}

// 행 이름을 레벨과 같게하여 FindRow함수에 행이름과 같은 레벨을 넣어서 행을 가져옴
FKRStatData* UKeroroGameInstance::GetKRStatData(int Level)
{
	return KRStatTable->FindRow<FKRStatData>(*FString::FromInt(Level), TEXT(""));
}

FCardData* UKeroroGameInstance::GetCardData(int CardID)
{
	return CardTable->FindRow<FCardData>(*FString::FromInt(CardID), TEXT(""));
}

FSoftObjectPath UKeroroGameInstance::GetFaceAssetPath(EKeroroType KeroroType, EFaceType FaceType)
{
	if (!FaceAssetTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("FaceAssetTable is null"));
		return FSoftObjectPath();
	}

	FString KeroroTypeName;
	switch (KeroroType)
	{
	case EKeroroType::Keroro: KeroroTypeName = TEXT("Keroro"); break;
	case EKeroroType::Tamama: KeroroTypeName = TEXT("Tamama"); break;
	case EKeroroType::Giroro: KeroroTypeName = TEXT("Giroro"); break;
	case EKeroroType::Kururu: KeroroTypeName = TEXT("Kururu"); break;
	case EKeroroType::Dororo: KeroroTypeName = TEXT("Dororo"); break;
	}

	FString FaceTypeName;
	switch (FaceType)
	{
	case EFaceType::Default: FaceTypeName = TEXT("Default"); break;
	case EFaceType::Sad: FaceTypeName = TEXT("Sad"); break;
	case EFaceType::Happy: FaceTypeName = TEXT("Happy"); break;
	case EFaceType::Fatal: FaceTypeName = TEXT("Fatal"); break;
	case EFaceType::Super: FaceTypeName = TEXT("Super"); break;
	case EFaceType::Anger: FaceTypeName = TEXT("Anger"); break;
	}

	FString RowNameStr = KeroroTypeName + TEXT("Face") + FaceTypeName;
	FName RowName(*RowNameStr);

	FKRFaceAssetData* FoundRow = FaceAssetTable->FindRow<FKRFaceAssetData>(RowName,TEXT(""));

	if (FoundRow)
	{
		return FoundRow->AssetPath;
	}

	UE_LOG(LogTemp, Warning, TEXT("Face asset not found ----- %s"), *RowNameStr);
	return FSoftObjectPath();
}

FStreamableManager& UKeroroGameInstance::GetStreamableManager()
{
	return StreamableManager;
}

FKRStatData::FKRStatData()
{
	Level = 1;
}

FCardData::FCardData()
{
	CardID = 1;
}

FKRFaceAssetData::FKRFaceAssetData()
{

}
