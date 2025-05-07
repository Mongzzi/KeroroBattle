// Fill out your copyright notice in the Description page of Project Settings.


#include "KeroroPlayerState.h"
#include "KeroroGameInstance.h"
#include "Kismet/GameplayStatics.h"

AKeroroPlayerState::AKeroroPlayerState()
{
	CurrentKeroro = EKeroroType::Keroro;
	TotalScore = 0;
	CurrentLevel = 7;
	CurrentGold = 0;
	CurrentExp = 0;
	KilledEnemyNum = 0;
}

void AKeroroPlayerState::BeginPlay()
{
	Super::BeginPlay();
	SetLevel(CurrentLevel);
	UE_LOG(LogTemp, Error, TEXT("setlevel in playerstate beginplay"));
}

EKeroroType AKeroroPlayerState::SetNextCharacterType()
{
	int32 NextType = static_cast<int32>(CurrentKeroro) + 1;

	if (NextType >= static_cast<int32>(EKeroroType::MAX))
	{
		CurrentKeroro = EKeroroType::Keroro;
	}
	else
	{
		CurrentKeroro = static_cast<EKeroroType>(NextType);
	}
	return CurrentKeroro;
}

bool AKeroroPlayerState::AddExp(int32 exp)
{
	CurrentGold += exp;
	return true;
}

float AKeroroPlayerState::GetExpRatio() const
{
	return 0.0f;
}


// exp 정보 가져오기위해서 스탯컴포넌트에서처럼 데이터테이블까지 가져옴
// max exp를 통해 레벨업이되었는가 , 현재 hp비율은 어느정도인가 알기위함
void AKeroroPlayerState::SetLevel(int32 lv)
{
	auto KRGameInstance = Cast<UKeroroGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (KRGameInstance != nullptr)
	{
		StatData = KRGameInstance->GetKRStatData(lv);
	}
	else
	{
		return;
	}

	if (StatData != nullptr)
	{
		CurrentLevel = lv;
		OnLevelChanged.Broadcast();
	}
}
