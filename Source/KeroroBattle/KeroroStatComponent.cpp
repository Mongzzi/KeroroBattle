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
	//SetLevel(Level);
}

// 계속 크래쉬나던이유 setlevel에서 데이터 가져오는데 beginplay에서
// setlevel해주고있었음 beginplay시점에는 이미 다른 곳에서 data를 필요로 해서 문제
// 따라서 컴포넌트 초기화할때 바로 데이터도 가져오는 형식으로 진행
void UKeroroStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
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

	UE_LOG(LogTemp, Warning, TEXT("===== PlayerState Stat Enhancement ====="));
	UE_LOG(LogTemp, Warning, TEXT("MaxHP_Enhanced: %f"), PlayerState->MaxHP_Enhanced);
	//UE_LOG(LogTemp, Warning, TEXT("MaxMP_Enhanced: %f"), PlayerState->MaxMP_Enhanced);
	//UE_LOG(LogTemp, Warning, TEXT("AttackSpeedRate_Enhanced: %f"), PlayerState->AttackSpeedRate_Enhanced);
	//UE_LOG(LogTemp, Warning, TEXT("MaxMoveSpeed_Enhanced: %f"), PlayerState->MaxMoveSpeed_Enhanced);
	//UE_LOG(LogTemp, Warning, TEXT("CritChanceRate_Enhanced: %f"), PlayerState->CritChanceRate_Enhanced);
	//UE_LOG(LogTemp, Warning, TEXT("SkillCooldownRate_Enhanced: %f"), PlayerState->SkillCooldownRate_Enhanced);
	//UE_LOG(LogTemp, Warning, TEXT("EvasionRate_Enhanced: %f"), PlayerState->EvasionRate_Enhanced);
	//UE_LOG(LogTemp, Warning, TEXT("DefenseRate_Enhanced: %f"), PlayerState->DefenseRate_Enhanced);
	//UE_LOG(LogTemp, Warning, TEXT("CritDamageRate_Enhanced: %f"), PlayerState->CritDamageRate_Enhanced);
	//UE_LOG(LogTemp, Warning, TEXT("ExpGainRate_Enhanced: %f"), PlayerState->ExpGainRate_Enhanced);
	//UE_LOG(LogTemp, Warning, TEXT("GoldGainRate_Enhanced: %f"), PlayerState->GoldGainRate_Enhanced);
	//UE_LOG(LogTemp, Warning, TEXT("ProjectileCount_Enhanced: %d"), PlayerState->ProjectileCount_Enhanced);
	//UE_LOG(LogTemp, Warning, TEXT("ProjectileScale_Enhanced: %f"), PlayerState->ProjectileScale_Enhanced);
	//UE_LOG(LogTemp, Warning, TEXT("InvincibilityTime_Enhanced: %f"), PlayerState->InvincibilityTime_Enhanced);
	//UE_LOG(LogTemp, Warning, TEXT("HealPowerRate_Enhanced: %f"), PlayerState->HealPowerRate_Enhanced);
	//UE_LOG(LogTemp, Warning, TEXT("HealPowerOnKill_Enhanced: %f"), PlayerState->HealPowerOnKill_Enhanced);
	
}

// 처음초기화 그리고 레벨업할 시 호출 예정
// 레벨초기화 해줄때 hud 업데이트도 델리게이트로 할 예정
// 적 캐릭터도 해당 스탯컴포넌트 사용중이라 주의 필요 , 추후 적 전용 스탯컴포넌트 생성 예정
void UKeroroStatComponent::SetLevel(int32 lv)
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
			AKeroroPlayerState* PS = kero->GetPlayerState<AKeroroPlayerState>();
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
				MaxHp = StatData->MaxHp;	// 플레이어는 UpdateStatCardEnhanced 통해서 해주고있음 // 적캐릭터는 카드강화 업데이트를 안해주기때문에 임시로 체력만 설정
				AttackPower = StatData->AttackPower;
				MaxMoveSpeed = StatData->MaxMoveSpeed;
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("kero is nullptr in stat component"));
			}
		}
	}
	SetHP(MaxHp);
}

void UKeroroStatComponent::SetDamage(float dm)
{
	
	if (MaxHp == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Stat Damage is failed"));
		//return;
	}
	SetHP(FMath::Clamp<float>(CurrentHp - dm, 0.0f, MaxHp));
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
	return (CurrentHp / MaxHp);
}

int32 UKeroroStatComponent::GetDropExp()
{
	return StatData->DropExp;
}

