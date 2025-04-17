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
	bIsRunning = false;

	// ¸ùÅ¸ÁÖ ·Îµå
	static ConstructorHelpers::FObjectFinder<UAnimMontage>SWORD_ATTACK_MONTAGE(TEXT("/Game/Animation/KR_Montage_Sword.KR_Montage_Sword"));
	if (SWORD_ATTACK_MONTAGE.Succeeded())SwordAttackMontage = SWORD_ATTACK_MONTAGE.Object;
}

void UKeroroAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

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

void UKeroroAnimInstance::PlayAttackMontage()
{
	if (!Montage_IsPlaying(SwordAttackMontage))
	{
		Montage_Play(SwordAttackMontage, 1.0f);
	}
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
	UE_LOG(LogTemp, Error, TEXT("Effect Create timing!"));
	OnEffectCreateCheck.Broadcast();
}

FName UKeroroAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	if (FMath::IsWithinInclusive<int32>(Section, 1, 4)) return FName(*FString::Printf(TEXT("Attack%d"), Section));
	else return NAME_None;
}

void UKeroroAnimInstance::JumptoAttackMontageSection(int32 NewSection)
{
	if (!Montage_IsPlaying(SwordAttackMontage)) return;
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), SwordAttackMontage);
}