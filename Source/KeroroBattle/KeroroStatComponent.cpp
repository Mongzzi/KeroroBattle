// Fill out your copyright notice in the Description page of Project Settings.


#include "KeroroStatComponent.h"
#include "KeroroGameInstance.h"
#include "KeroroPlayerState.h"
#include "KeroroCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UKeroroStatComponent::UKeroroStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

	Level = 1;

	HealPowerRate = 0.0f;
	HealPowerRate_Default = 0.0f;

	HealPowerOnKill = 0.0f;
	HealPowerOnKill_Default = 0.0f;

	ExpGainRate = 1.0f;
	ExpGainRate_Default = 1.0f;

	GoldGainRate = 1.0f;
	GoldGainRate_Default = 1.0f;

	ProjectileCount = 1;
	ProjectileCount_Default = 1;

	ProjectileScale = 1.0f;
	ProjectileScale_Default = 1.0f;

	InvincibilityTime = 0.0f;
	InvincibilityTime_Default = 0.0f;

	CritDamageRate = 1.5f;
	CritDamageRate_Default = 1.5f;
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

void UKeroroStatComponent::UpdateStatCardEnhanced(AKeroroPlayerState* PlayerState)
{
	if (!PlayerState)
	{
		UE_LOG(LogTemp, Error, TEXT("PS is nullptr"));
		return;
	}

	if (!StatData)
	{
		UE_LOG(LogTemp, Error, TEXT("StatData is nullptr"));
		return;
	}
	// 스탯데이터테이블 존재 스탯
	AttackPower = StatData->AttackPower;
	DropExp = StatData->DropExp;
	NextExp = StatData->NextExp;

	MaxHp = StatData->MaxHp + PlayerState->MaxHP_Enhanced;
	MaxMp = StatData->MaxMP + PlayerState->MaxMP_Enhanced;
	AttackSpeedRate = StatData->AttackSpeedRate + PlayerState->AttackSpeedRate_Enhanced;
	MaxMoveSpeed = StatData->MaxMoveSpeed + PlayerState->MaxMoveSpeed_Enhanced;
	CritChanceRate = StatData->CritChanceRate + PlayerState->CritChanceRate_Enhanced;
	SkillCooldownRate = StatData->SkillCooldownRate + PlayerState->SkillCooldownRate_Enhanced;
	EvasionRate = PlayerState->EvasionRate_Enhanced;
	DefenseRate += PlayerState->DefenseRate_Enhanced;

	// 스탯데이터테이블 존재 x 스탯
	CritDamageRate = CritDamageRate_Default + PlayerState->CritDamageRate_Enhanced;
	ExpGainRate = ExpGainRate_Default + PlayerState->ExpGainRate_Enhanced;
	GoldGainRate = GoldGainRate_Default + PlayerState->GoldGainRate_Enhanced;
	ProjectileCount = ProjectileCount_Default + PlayerState->ProjectileCount_Enhanced;
	ProjectileScale = ProjectileScale_Default + PlayerState->ProjectileScale_Enhanced;
	InvincibilityTime = InvincibilityTime_Default + PlayerState->InvincibilityTime_Enhanced;
	HealPowerRate = HealPowerRate_Default + PlayerState->HealPowerRate_Enhanced;
	HealPowerOnKill = HealPowerOnKill_Default + PlayerState->HealPowerOnKill_Enhanced;
}

// 처음초기화 그리고 레벨업할 시 호출 예정
// 레벨초기화 해줄때 hud 업데이트도 델리게이트로 할 예정
void UKeroroStatComponent::SetLevel(int32 lv)
{
	//UE_LOG(LogTemp, Warning, TEXT("Set level in statcomp"));

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
		AKeroroCharacter* kero = GetOwner<AKeroroCharacter>();
		if (kero)
		{
			AKeroroPlayerState* PS = kero->GetPlayerState<AKeroroPlayerState>();
			if (PS)
			{
				// 카드 수치 반영해서 스탯 업데이트
				UpdateStatCardEnhanced(PS);
			}
			else {
				UE_LOG(LogTemp, Error, TEXT("PS is nullptr in stat component"));
			}
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("kero is nullptr in stat component"));
		}
	}
}

void UKeroroStatComponent::SetDamage(float dm)
{
	if (StatData == nullptr)
	{
		//UE_LOG(LogTemp, Error, TEXT("Stat Damage is nullptr - in setdamage"));
		return;
	}
	SetHP(FMath::Clamp<float>(CurrentHp - dm, 0.0f, StatData->MaxHp));
}

void UKeroroStatComponent::SetHP(float hp)
{
	CurrentHp = hp;
	OnHpIsChanged.Broadcast();
	if (CurrentHp <= 0.0f)
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
	return (CurrentHp / StatData->MaxHp);
}

int32 UKeroroStatComponent::GetDropExp()
{
	return StatData->DropExp;
}

