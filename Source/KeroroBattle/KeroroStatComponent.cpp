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
	Level = 5;

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
		UE_LOG(LogTemp, Warning, TEXT("set level success"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("set level error"));
		return;
	}

	if (StatData != nullptr)
	{
		Level = lv;
		CurrentHP = StatData->MaxHp;
		AttackPower = StatData->AttackPower;
		UE_LOG(LogTemp, Warning, TEXT("set level success"));
	}
}

void UKeroroStatComponent::SetDamage(float dm)
{
	if (StatData == nullptr) return;
	CurrentHP = FMath::Clamp<float>(CurrentHP - dm, 0.0f, StatData->MaxHp);
	if (CurrentHP <= 0.0f) OnHpIsZero.Broadcast(); // 체력 0이하가되면 바인딩된 객체들 함수 호출

}


