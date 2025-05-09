// Fill out your copyright notice in the Description page of Project Settings.


#include "KeroroPlayerState.h"
#include "KeroroGameInstance.h"
#include "Kismet/GameplayStatics.h"

AKeroroPlayerState::AKeroroPlayerState()
{
	CurrentKeroro = EKeroroType::Keroro;
	TotalScore = 0;
	CurrentLevel = 18;
	CurrentGold = 0;
	CurrentExp = 0;
	KilledEnemyNum = 0;
}

void AKeroroPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SetLevel(CurrentLevel);
}

void AKeroroPlayerState::BeginPlay()
{
	Super::BeginPlay();

	// 처음 게임 시작시 플레이어레벨설정
	// 함수안에서 바인딩된 함수들 호출
	// 현재 태그중인 캐릭터들 스탯 업데이트
	// hud 레벨 부분 업데이트
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
	if (StatData->NextExp == -1)
		return false;

	bool DidLevelUp = false;
	CurrentExp += exp;
	if (CurrentExp >= StatData->NextExp)
	{
		CurrentExp -= StatData->NextExp;
		SetLevel(CurrentLevel + 1);
		DidLevelUp = true;
	}
	OnExpChanged.Broadcast();
	if (DidLevelUp) {
		OnLevelChanged.Broadcast();
	}
	UE_LOG(LogTemp, Error, TEXT("EXP = %d"),CurrentExp);

	return DidLevelUp;
}

float AKeroroPlayerState::GetExpRatio() const
{
	if (StatData == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("StatData nullptr in ps getexpratio"));
		return 0.0f;
	}

	if (StatData->NextExp <= 0.0f) return 0.0f;

	float Result = (float)CurrentExp / (float)StatData->NextExp;
	return Result;
}

void AKeroroPlayerState::AddKillEnemyNum()
{
	KilledEnemyNum += 1;
	OnKillNumChanged.Broadcast();
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
