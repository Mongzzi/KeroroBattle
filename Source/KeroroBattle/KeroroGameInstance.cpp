// Fill out your copyright notice in the Description page of Project Settings.


#include "KeroroGameInstance.h"

UKeroroGameInstance::UKeroroGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable>KRDT(TEXT("/Game/GameData/StatData.StatData"));
	if (KRDT.Succeeded())KRStatTable = KRDT.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable>CDDT(TEXT("/Game/GameData/CardData.CardData"));
	if (CDDT.Succeeded())CardTable = CDDT.Object;

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

FKRStatData::FKRStatData()
{
	Level = 1;
}

FCardData::FCardData()
{
	CardID = 1;
}
