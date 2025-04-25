// Fill out your copyright notice in the Description page of Project Settings.


#include "KeroroStatComponent.h"
#include "KeroroGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UKeroroStatComponent::UKeroroStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
	Level = 1;

}


// Called when the game starts
void UKeroroStatComponent::BeginPlay()
{
	Super::BeginPlay();
	SetLevel(Level);

}

// 처음초기화 그리고 레벨업할 시 호출 예정
void UKeroroStatComponent::SetLevel(int32 lv)
{
	auto KRGameInstance = Cast<UKeroroGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (KRGameInstance != nullptr)
	{
		// 레벨에따른 스탯정보 가져옴
		StatData = KRGameInstance->GetKRStatData(lv);
		UE_LOG(LogTemp, Warning, TEXT("Get StatData from GameInstance is Success"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Get StatData from GameInstance is failed"));
		return;
	}

	if (StatData != nullptr)
	{
		Level = lv;
		SetHP(StatData->MaxHp);
		AttackPower = StatData->AttackPower;
		UE_LOG(LogTemp, Warning, TEXT("set level success"));
	}
}

void UKeroroStatComponent::SetDamage(float dm)
{
	if (StatData == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Stat Damage is nullptr - in setdamage"));
		return;
	}
	SetHP(FMath::Clamp<float>(CurrentHP - dm, 0.0f, StatData->MaxHp));
}

void UKeroroStatComponent::SetHP(float hp)
{
	CurrentHP = hp;
	OnHpIsChanged.Broadcast();
	if (CurrentHP <= 0.0f)
	{
		OnHpIsZero.Broadcast();
	}
}

float UKeroroStatComponent::GetHpRatio()
{
	return (CurrentHP / StatData->MaxHp);
}


