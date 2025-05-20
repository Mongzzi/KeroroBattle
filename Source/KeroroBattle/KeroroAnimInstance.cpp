// Fill out your copyright notice in the Description page of Project Settings.


#include "KeroroAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimNotifies/AnimNotify.h"

UKeroroAnimInstance::UKeroroAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	IsInAir = false;
	IsDead = false;
	bIsRunning = false;
	WeaponType = EWeaponType::SWORD;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SWORD_ATTACK_MONTAGE(TEXT("/Game/Animation/KR_Montage_Sword.KR_Montage_Sword"));
	if (SWORD_ATTACK_MONTAGE.Succeeded())
	{
		SwordAttackMontage = SWORD_ATTACK_MONTAGE.Object;
		AttackMontages.Add(EWeaponType::SWORD, SwordAttackMontage);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> RIFLE_ATTACK_MONTAGE(TEXT("/Game/Animation/KR_Montage_Rifle.KR_Montage_Rifle"));
	if (RIFLE_ATTACK_MONTAGE.Succeeded())
	{
		RifleAttackMontage = RIFLE_ATTACK_MONTAGE.Object;
		AttackMontages.Add(EWeaponType::RIFLE, RifleAttackMontage);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> KEROBALL_ATTACK_MONTAGE(TEXT("/Game/Animation/KR_Montage_KeroBall.KR_Montage_KeroBall"));
	if (KEROBALL_ATTACK_MONTAGE.Succeeded())
	{
		KeroBallAttackMontage = KEROBALL_ATTACK_MONTAGE.Object;
		AttackMontages.Add(EWeaponType::KEROBALL, KeroBallAttackMontage);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> FIST_ATTACK_MONTAGE(TEXT("/Game/Animation/KR_Montage_Fist.KR_Montage_Fist"));
	if (FIST_ATTACK_MONTAGE.Succeeded())
	{
		FistAttackMontage = FIST_ATTACK_MONTAGE.Object;
		AttackMontages.Add(EWeaponType::FIST, FistAttackMontage);
	}
}

void UKeroroAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!IsDead)
	{
		auto Pawn = TryGetPawnOwner();
		if (IsValid(Pawn))
		{
			CurrentPawnSpeed = Pawn->GetVelocity().Size();
		}
		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			IsInAir = Character->GetMovementComponent()->IsFalling();
		}
	}

}

void UKeroroAnimInstance::PlayAttackMontage()
{
	if (IsDead) return;
	if (UAnimMontage* Montage = GetWeaponMontage())
	{
		if (!Montage_IsPlaying(Montage))
		{
			Montage_Play(Montage, 1.0f);
			UE_LOG(LogTemp, Error, TEXT(" play attack montage called"));

		}
	}
}

void UKeroroAnimInstance::SetDeadAnim()
{
	IsDead = true;
}

void UKeroroAnimInstance::SetWeaponType(EWeaponType type)
{
	WeaponType = type;
}

UAnimMontage* UKeroroAnimInstance::GetWeaponMontage()
{
	if (UAnimMontage** FoundPtr = AttackMontages.Find(WeaponType))
	{
		return *FoundPtr;
	}
	return nullptr;
}

void UKeroroAnimInstance::AnimNotify_AttackHitCheck()
{
	OnAttackHitCheck.Broadcast();
}

void UKeroroAnimInstance::AnimNotify_NextAttackCheck()
{
	OnNextAttackCheck.Broadcast();
}

void UKeroroAnimInstance::AnimNotify_EffectCreateCheck()
{
	//UE_LOG(LogTemp, Error, TEXT("Effect Create timing!"));
	OnEffectCreateCheck.Broadcast();
}

FName UKeroroAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	if (FMath::IsWithinInclusive<int32>(Section, 1, 4)) return FName(*FString::Printf(TEXT("Attack%d"), Section));
	else return NAME_None;
}

void UKeroroAnimInstance::JumptoAttackMontageSection(int32 NewSection)
{
	if (IsDead) return;

	if (UAnimMontage* MontageToPlay = GetWeaponMontage())
	{
		if (Montage_IsPlaying(MontageToPlay))
		{
			Montage_JumpToSection(GetAttackMontageSectionName(NewSection), MontageToPlay);
		}
	}
}