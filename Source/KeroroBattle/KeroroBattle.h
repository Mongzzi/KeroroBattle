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
enum class ESoundType : uint8
{
	ComboAttack				UMETA(DisplayName = "ComboAttack"),
	Die						UMETA(DisplayName = "Die"),
	MAX						UMETA(Hidden) // 순환 계산용
};


UENUM(BlueprintType)
enum class ECardType : uint8
{
	MaxHP					UMETA(DisplayName = "MaxHP"),						// 최대 체력
	MaxMP					UMETA(DisplayName = "MaxMP"),						// 최대 마나
	AttackSpeed				UMETA(DisplayName = "AttackSpeed"),					// 공격속도
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
	HealAmount				UMETA(DisplayName = "HealAmount"),					// 체력회복
	HealOnKill				UMETA(DisplayName = "HealOnKill"),					// 공격시 체력회복
	Shield					UMETA(DisplayName = "Shield"),						// 쉴드(+패링) 시간
	ShieldCoolTime			UMETA(DisplayName = "ShieldCoolTime"),				// 쉴드 쿨타임 시간
	MAX						UMETA(Hidden)										// 순환 계산용
};