// Fill out your copyright notice in the Description page of Project Settings.


#include "KeroroStatComponent.h"
#include "KeroroGameInstance.h"
#include "KeroroPlayerState.h"
#include "KeroroCharacter.h"
#include "KeroroEnemyCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UKeroroStatComponent::UKeroroStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

	Level = 1;
	UltiCostMp = 80.0f;

	HealPowerRate = 0.03f;
	HealPowerRate_Default = 0.03f;

	HealPowerOnKill = 0.0f;
	HealPowerOnKill_Default = 0.0f;

	ProjectileCount = 1;
	ProjectileCount_Default = 1;

	ProjectileScale = 1.0f;
	ProjectileScale_Default = 1.0f;

	InvincibilityTime = 1.5f;
	InvincibilityTime_Default = 1.5f;

	CritDamageRate = 1.5f;
	CritDamageRate_Default = 1.5f;

	// 테스트용 시간 3~4초 정도 적당한듯 추후 변경해줘야함
	GuardCoolTime = 3.0f;
	GuardCollTime_Default = 3.0f;

	GuardTime = 0.5f;
	GuardTime_Default = 0.5f;

	ParryTime = 0.15f;	//0.15가 맞음
	ParryTime_Default = 0.15f;

	// 데이터 테이블 없는 항목
	HealIntervalTime = 5.0f;
}


// Called when the game starts
void UKeroroStatComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UKeroroStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UKeroroStatComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
	}
}

void UKeroroStatComponent::UpdateStatCardEnhanced(AKeroroPlayerState* PlayerState)
{
	if (!PlayerState)
	{
		UE_LOG(LogTemp, Error, TEXT("PS is nullptr in UpdateStatCardEnhanced"));
		return;
	}

	if (!StatData)
	{
		UE_LOG(LogTemp, Error, TEXT("StatData is nullptr in UpdateStatCardEnhanced"));
		return;
	}
	// 스탯데이터테이블 존재 스탯
	AttackPower = StatData->AttackPower;
	DropExp = StatData->DropExp;
	NextExp = StatData->NextExp;

	MaxHp = StatData->MaxHp + PlayerState->MaxHP_Enhanced;
	MaxMp = StatData->MaxMP + PlayerState->MaxMP_Enhanced;
	//CurrentHp = MaxHp;
	//CurrentMp = MaxMp;


	MaxMoveSpeed = StatData->MaxMoveSpeed + PlayerState->MaxMoveSpeed_Enhanced;
	CritChanceRate = StatData->CritChanceRate + PlayerState->CritChanceRate_Enhanced;
	SkillCooldownRate = StatData->SkillCooldownRate + PlayerState->SkillCooldownRate_Enhanced;
	EvasionRate = StatData->EvasionRate + PlayerState->EvasionRate_Enhanced;
	DefenseRate = StatData->DefenseRate + PlayerState->DefenseRate_Enhanced;

	// 스탯데이터테이블 존재 x 스탯
	CritDamageRate = CritDamageRate_Default + PlayerState->CritDamageRate_Enhanced;
	ProjectileCount = ProjectileCount_Default + PlayerState->ProjectileCount_Enhanced;
	ProjectileScale = ProjectileScale_Default + PlayerState->ProjectileScale_Enhanced;
	InvincibilityTime = InvincibilityTime_Default + PlayerState->InvincibilityTime_Enhanced;
	HealPowerRate = HealPowerRate_Default + PlayerState->HealPowerRate_Enhanced;
	HealPowerOnKill = HealPowerOnKill_Default + PlayerState->HealPowerOnKill_Enhanced;

	GuardCoolTime = GuardCollTime_Default * (1.0f - PlayerState->GuardCoolTime_Enhanced);
	if (GuardCoolTime < 0.0f) GuardCoolTime = 0.001f;
	GuardTime = GuardTime_Default + PlayerState->GuardTime_Enhanced;
	ParryTime = ParryTime_Default + PlayerState->ParryTime_Enhanced;


	{
		//UE_LOG(LogTemp, Warning, TEXT("===== PlayerState Stat Enhancement ====="));
		//UE_LOG(LogTemp, Warning, TEXT("MaxHP_Enhanced: %f"), PlayerState->MaxHP_Enhanced);
		//UE_LOG(LogTemp, Warning, TEXT("MaxMP_Enhanced: %f"), PlayerState->MaxMP_Enhanced);
		//UE_LOG(LogTemp, Warning, TEXT("AttackSpeedRate_Enhanced: %f"), PlayerState->AttackSpeedRate_Enhanced);
		//UE_LOG(LogTemp, Warning, TEXT("MaxMoveSpeed_Enhanced: %f"), PlayerState->MaxMoveSpeed_Enhanced);
		//UE_LOG(LogTemp, Warning, TEXT("CritChanceRate_Enhanced: %f"), PlayerState->CritChanceRate_Enhanced);
		//UE_LOG(LogTemp, Warning, TEXT("SkillCooldownRate_Enhanced: %f"), PlayerState->SkillCooldownRate_Enhanced);
		//UE_LOG(LogTemp, Warning, TEXT("EvasionRate_Enhanced: %f"), PlayerState->EvasionRate_Enhanced);
		//UE_LOG(LogTemp, Warning, TEXT("DefenseRate_Enhanced: %f"), PlayerState->DefenseRate_Enhanced);
		//UE_LOG(LogTemp, Warning, TEXT("CritDamageRate_Enhanced: %f"), PlayerState->CritDamageRate_Enhanced);
		////UE_LOG(LogTemp, Warning, TEXT("ExpGainRate_Enhanced: %f"), PlayerState->ExpGainRate_Enhanced);
		////UE_LOG(LogTemp, Warning, TEXT("GoldGainRate_Enhanced: %f"), PlayerState->GoldGainRate_Enhanced);
		//UE_LOG(LogTemp, Warning, TEXT("ProjectileCount_Enhanced: %d"), PlayerState->ProjectileCount_Enhanced);
		//UE_LOG(LogTemp, Warning, TEXT("ProjectileScale_Enhanced: %f"), PlayerState->ProjectileScale_Enhanced);
		//UE_LOG(LogTemp, Warning, TEXT("InvincibilityTime_Enhanced: %f"), PlayerState->InvincibilityTime_Enhanced);
		//UE_LOG(LogTemp, Warning, TEXT("HealPowerRate_Enhanced: %f"), PlayerState->HealPowerRate_Enhanced);
		//UE_LOG(LogTemp, Warning, TEXT("HealPowerOnKill_Enhanced: %f"), PlayerState->HealPowerOnKill_Enhanced);
		//UE_LOG(LogTemp, Warning, TEXT("GuardCoolTime: %f"), GuardCoolTime);
	}

}

// 처음초기화 그리고 레벨업할 시 호출 예정
// 레벨초기화 해줄때 hud 업데이트도 델리게이트로 할 예정
// 적 캐릭터도 해당 스탯컴포넌트 사용중이라 주의 필요 , 추후 적 전용 스탯컴포넌트 생성 예정
void UKeroroStatComponent::SetLevel(int32 lv, AKeroroPlayerState* PlayerState)
{
	//UE_LOG(LogTemp, Warning, TEXT("Set level in statcomp"));

	auto KRGameInstance = Cast<UKeroroGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (KRGameInstance != nullptr)
	{
		Level = lv;
		StatData = KRGameInstance->GetKRStatData(Level);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("KRGameInstance is nullptr in stat component"));
		return;
	}

	if (StatData != nullptr)
	{
		AKeroroCharacter* kero = GetOwner<AKeroroCharacter>();
		if (kero)	// 플레이어 캐릭터일때
		{
			AKeroroPlayerState* PS;
			if (PlayerState == nullptr)
			{
				PS = kero->GetPlayerState<AKeroroPlayerState>();

			}
			else
			{
				PS = PlayerState;
			}

			if (PS)
			{
				// 플레이어스테이트에서 관리 중인 카드 수치 반영해서 스탯 업데이트
				UpdateStatCardEnhanced(PS);
			}
			else {
				UE_LOG(LogTemp, Error, TEXT("PS is nullptr in stat component"));
			}

		}
		else {	// 적 캐릭터일때
			AKeroroEnemyCharacter* enemy = GetOwner<AKeroroEnemyCharacter>();
			if (enemy)
			{
				AttackPower = StatData->AttackPower;
				DropExp = StatData->DropExp;
				NextExp = StatData->NextExp;

				MaxHp = StatData->MaxHp;
				MaxMp = StatData->MaxMP;
				MaxMoveSpeed = StatData->MaxMoveSpeed;
				CritChanceRate = StatData->CritChanceRate;
				SkillCooldownRate = StatData->SkillCooldownRate;
				EvasionRate = StatData->EvasionRate;
				DefenseRate = StatData->DefenseRate;
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("kero is nullptr in stat component"));
			}
		}
	}

	SetHP(MaxHp);
	SetMP(MaxMp);
}

void UKeroroStatComponent::SetDamage(float Damage)
{

	if (MaxHp == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Stat Damage is failed"));
		//return;
	}
	SetHP(FMath::Clamp<float>(CurrentHp - Damage, 0.0f, MaxHp));
}

float UKeroroStatComponent::SetFinalDamage(float Damage)
{
	if (bIsInvincible)
	{
		UE_LOG(LogTemp, Warning, TEXT("invisible time ~~"));
		return 0.0f;
	}

	// 회피 판정
	if (FMath::FRand() < EvasionRate)
	{
		UE_LOG(LogTemp, Warning, TEXT("evade ~~"));
		return 0.0f;
	}

	// 방어율 적용
	float FinalDamage = Damage * (1.0f - DefenseRate);

	// 데미지 적용
	SetDamage(FinalDamage);

	// 무적 시간 시작
	StartInvincibility();

	return FinalDamage;
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

void UKeroroStatComponent::SetMP(float mp)
{
	CurrentMp = mp;
	OnMpIsChanged.Broadcast();
}

float UKeroroStatComponent::GetHpRatio()
{
	return (CurrentHp / MaxHp);
}

float UKeroroStatComponent::GetMpRatio()
{
	UE_LOG(LogTemp, Error, TEXT("Cur MP = %f, MaxMp = %f"), CurrentMp, MaxMp);
	return (CurrentMp / MaxMp);
}

int32 UKeroroStatComponent::GetDropExp()
{
	return StatData->DropExp;
}

// 추후 데이터 테이블 사용하여 조정
int32 UKeroroStatComponent::GetDropGold()
{
	if (Level >= 1 && Level < 10)
	{
		return 10;
	}
	else if (Level >= 10 && Level < 21)
	{
		return 30;
	}
	return 0;
}

void UKeroroStatComponent::StartAutoHeal()
{
	GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, [this]() {
		CurrentHp = FMath::Clamp(CurrentHp + MaxHp * HealPowerRate, 0.0f, MaxHp);
		//UE_LOG(LogTemp, Log, TEXT("Auto heal amount : %f, current hp = %f"), MaxHp * HealPowerRate, CurrentHp);
		OnHpIsChanged.Broadcast();
		},
		HealIntervalTime,
		true);
}

void UKeroroStatComponent::StartAutoMpHeal()
{
	//GetWorld()->GetTimerManager().SetTimer(MpHealTimerHandle, [this]() {
	//	CurrentMp = FMath::Clamp(CurrentMp + MaxMp * MpHealPowerRate, 0.0f, MaxHp);
	//	//UE_LOG(LogTemp, Log, TEXT("Auto heal amount : %f, current hp = %f"), MaxHp * HealPowerRate, CurrentHp);
	//	OnHpIsChanged.Broadcast();
	//	},
	//	HealIntervalTime,
	//	true);
}

void UKeroroStatComponent::AttackHeal()
{
	if (HealPowerOnKill > 0.0f)
	{
		CurrentHp = FMath::Clamp(CurrentHp + MaxHp * HealPowerOnKill, 0.0f, MaxHp);
		UE_LOG(LogTemp, Log, TEXT("Attack heal amount : %f, current hp = %f"), MaxHp * HealPowerOnKill, CurrentHp);
		OnHpIsChanged.Broadcast();
	}
}

void UKeroroStatComponent::AttackMpHeal()
{
}

void UKeroroStatComponent::StartInvincibility()
{
	bIsInvincible = true;

	GetWorld()->GetTimerManager().SetTimer(
		InvincibilityTimerHandle,
		this,
		&UKeroroStatComponent::EndInvincibility,
		InvincibilityTime,
		false
	);

	UE_LOG(LogTemp, Log, TEXT("invisible time start %f seconds"), InvincibilityTime);
}

void UKeroroStatComponent::EndInvincibility()
{
	bIsInvincible = false;
	UE_LOG(LogTemp, Log, TEXT("invisible time end....."));
}

bool UKeroroStatComponent::PlayUltiSkill()
{
	float cal = CurrentMp - UltiCostMp;
	if (cal < 0.0f) return false;
	else
	{
		CurrentMp -= UltiCostMp;
		OnMpIsChanged.Broadcast();
		return true;
	}
}

