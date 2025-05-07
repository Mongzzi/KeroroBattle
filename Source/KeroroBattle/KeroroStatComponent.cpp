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
}

// 계속 크래쉬나던이유 setlevel에서 데이터 가져오는데 beginplay에서
// setlevel해주고있었음 beginplay시점에는 이미 다른 곳에서 data를 필요로 해서 문제
// 따라서 컴포넌트 초기화할때 바로 데이터도 가져오는 형식으로 진행
void UKeroroStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetLevel(Level);
}

// 처음초기화 그리고 레벨업할 시 호출 예정
// 레벨초기화 해줄때 hud 업데이트도 델리게이트로 할 예정
void UKeroroStatComponent::SetLevel(int32 lv)
{
	UE_LOG(LogTemp, Warning, TEXT("Set level in statcomp"));

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
		Level = lv;
		SetHP(StatData->MaxHp);
		AttackPower = StatData->AttackPower;
		

	}
}

void UKeroroStatComponent::SetDamage(float dm)
{
	if (StatData == nullptr)
	{
		//UE_LOG(LogTemp, Error, TEXT("Stat Damage is nullptr - in setdamage"));
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
	if (StatData == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Stat Data is nullptr - in GetHpRatio"));
		return 0.0f;
	}
	return (CurrentHP / StatData->MaxHp);
}


