// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


UENUM(BlueprintType)
enum class EKeroroType : uint8
{
	Keroro      UMETA(DisplayName = "Keroro"),
	Tamama      UMETA(DisplayName = "Tamama"),
	Giroro      UMETA(DisplayName = "Giroro"),
	Kururu      UMETA(DisplayName = "Kururu"),
	Dororo      UMETA(DisplayName = "Dororo"),
	MAX         UMETA(Hidden) // 순환 계산용
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	KEROBALL     UMETA(DisplayName = "KEROBALL"),
	RIFLE		 UMETA(DisplayName = "RIFLE"),
	SWORD		 UMETA(DisplayName = "SWORD"),
	FIST		 UMETA(DisplayName = "FIST"),
	NOTEBOOK	 UMETA(DisplayName = "NOTEBOOK"),
	MAX			 UMETA(Hidden) // 순환 계산용
};

UENUM(BlueprintType)
enum class EFaceType : uint8
{
	Default			UMETA(DisplayName = "Default"),
	Sad				UMETA(DisplayName = "Sad"),
	Happy			UMETA(DisplayName = "Happy"),
	Fatal			UMETA(DisplayName = "Fatal"),
	Super			UMETA(DisplayName = "Super"),
	Anger			UMETA(DisplayName = "Anger"),
	MAX				UMETA(Hidden) // 순환 계산용
};

UENUM(BlueprintType)
enum class EKRSoundType : uint8
{
	ComboAttack				UMETA(DisplayName = "ComboAttack"),
	Die						UMETA(DisplayName = "Die"),
	Ultimate				UMETA(DisplayName = "Ultimate"),
	Hitted					UMETA(DisplayName = "Hitted"),
	MAX						UMETA(Hidden) // 순환 계산용
};

UENUM(BlueprintType)
enum class EWidgetType : uint8
{
	SKILL_IMAGE				UMETA(DisplayName = "SKILL_IMAGE"),
	SHIELD_IMAGE			UMETA(DisplayName = "SHIELD_IMAGE"),
	ITEM_IMAGE				UMETA(DisplayName = "ITEM_IMAGE")
};

UENUM(BlueprintType)

enum class EItemType : uint8
{
	None					UMETA(DisplayName = "None"),
	HP						UMETA(DisplayName = "HP Recovery"),             // 1. 체력 회복
	MP						UMETA(DisplayName = "MP Recovery"),             // 2. 마나 회복
	ShieldCoolZero			UMETA(DisplayName = "Shield Cooldown 0"),       // 3. 쉴드 쿨타임 0
	UltCoolZero				UMETA(DisplayName = "Ultimate Cooldown 0"),     // 4. 궁극기 쿨타임 0
	Gold					UMETA(DisplayName = "Gold Gain"),               // 5. 골드 획득
	KillAll					UMETA(DisplayName = "Kill All Enemies"),        // 6. 적 전체 처치
	GroupEnemies			UMETA(DisplayName = "Group All Enemies"),       // 7. 적 한곳에 몰기
	LevelUp					UMETA(DisplayName = "Level Up"),                // 8. 레벨업
	AttackUp				UMETA(DisplayName = "Attack Up"),               // 9. 공격력 업
	MoveSpeedUp				UMETA(DisplayName = "Move Speed Up"),           // 10. 이동속도 업
	DefenseUp				UMETA(DisplayName = "Defense Up"),              // 11. 방어력 업
	MAX						UMETA(Hidden)
};


UENUM(BlueprintType)
enum class ECardType : uint8
{
	MaxHP					UMETA(DisplayName = "MaxHP"),						// 최대 체력
	MaxMP					UMETA(DisplayName = "MaxMP"),						// 최대 마나
	MoveSpeed				UMETA(DisplayName = "MoveSpeed"),					// 이동속도
	CriticalChance			UMETA(DisplayName = "CriticalChance"),				// 치명타확률
	CriticalDamage			UMETA(DisplayName = "CriticalDamage"),				// 치명타 데미지
	EXPBoost				UMETA(DisplayName = "EXPBoost"),					// 경험치획득량
	GoldBoost				UMETA(DisplayName = "GoldBoost"),					// 골드획득량
	CooldownReduction		UMETA(DisplayName = "CooldownReduction"),			// 쿨타임감소
	ProjectileCount			UMETA(DisplayName = "ProjectileCount"),				// 발사체 속도
	ProjectileSize			UMETA(DisplayName = "ProjectileSize"),				// 발사체 크기
	Evasion					UMETA(DisplayName = "Evasion"),						// 회피율
	InvincibilityTime		UMETA(DisplayName = "InvincibilityTime"),			// 무적시간
	Armor					UMETA(DisplayName = "Armor"),						// 방어력
	HealAmount				UMETA(DisplayName = "HealAmount"),					// 체력자동회복
	HealOnKill				UMETA(DisplayName = "HealOnKill"),					// 처치 시 체력회복
	Shield					UMETA(DisplayName = "Shield"),						// 쉴드(+패링) 시간
	ShieldCoolTime			UMETA(DisplayName = "ShieldCoolTime"),				// 쉴드 쿨타임 시간
	MpHealAmount			UMETA(DisplayName = "MpHealAmount"),				// 마나자동회복
	MpHealOnKill			UMETA(DisplayName = "MpHealOnKill"),				// 처치 시 마나회복
	MAX						UMETA(Hidden)										// 순환 계산용
};

enum EDamageResultType
{
	Normal,
	Evaded,
	Invincible
};

struct FDamageResult
{
	EDamageResultType ResultType = EDamageResultType::Normal;
	float FinalDamage = 0.0f;

	FDamageResult() {}
	FDamageResult(EDamageResultType Type, float Damage) : ResultType(Type), FinalDamage(Damage) {}
};