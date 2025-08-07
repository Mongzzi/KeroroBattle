// Fill out your copyright notice in the Description page of Project Settings.


#include "KeroroAnimInstance.h"
#include "KeroroCharacter.h"
#include "KeroroPlayerState.h"
#include "KeroroStatComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimSequence.h"
#include "Animation/AnimNotifies/AnimNotify.h"

UKeroroAnimInstance::UKeroroAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	IsInAir = false;
	IsDead = false;
	bIsGuarding = false;
	bIsRunning = false;
	bIsRolling = false;
	bIsUltiSkillPlaying = false;
	AnimationRunSpeed = 1.0f;
	WeaponType = EWeaponType::SWORD;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SWORD_ATTACK_MONTAGE(TEXT("/Game/Animation/KR_Montage_Sword.KR_Montage_Sword"));
	if (SWORD_ATTACK_MONTAGE.Succeeded())
	{
		SwordAttackMontage = SWORD_ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> RIFLE_ATTACK_MONTAGE(TEXT("/Game/Animation/KR_Montage_Rifle.KR_Montage_Rifle"));
	if (RIFLE_ATTACK_MONTAGE.Succeeded())
	{
		RifleAttackMontage = RIFLE_ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> KEROBALL_ATTACK_MONTAGE(TEXT("/Game/Animation/KR_Montage_KeroBall.KR_Montage_KeroBall"));
	if (KEROBALL_ATTACK_MONTAGE.Succeeded())
	{
		KeroBallAttackMontage = KEROBALL_ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> FIST_ATTACK_MONTAGE(TEXT("/Game/Animation/KR_Montage_Fist.KR_Montage_Fist"));
	if (FIST_ATTACK_MONTAGE.Succeeded())
	{
		FistAttackMontage = FIST_ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> NOTEBOOK_ATTACK_MONTAGE(TEXT("/Game/Animation/KR_Montage_NoteBook.KR_Montage_NoteBook"));
	if (NOTEBOOK_ATTACK_MONTAGE.Succeeded())
	{
		NoteBookAttackMontage = NOTEBOOK_ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ULTIMONTAGE1(TEXT("/Game/Animation/Doro_UltiMontage.Doro_UltiMontage"));
	if (ULTIMONTAGE1.Succeeded())
	{
		SwordUltiMontage = ULTIMONTAGE1.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ULTIMONTAGE2(TEXT("/Game/Animation/Giro_UltiMontage.Giro_UltiMontage"));
	if (ULTIMONTAGE2.Succeeded())
	{
		RifleUltiMontage = ULTIMONTAGE2.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ULTIMONTAGE3(TEXT("/Game/Animation/Kero_UltiMontage.Kero_UltiMontage"));
	if (ULTIMONTAGE3.Succeeded())
	{
		KeroBallUltiMontage = ULTIMONTAGE3.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ULTIMONTAGE4(TEXT("/Game/Animation/Tama_UltiMontage.Tama_UltiMontage"));
	if (ULTIMONTAGE4.Succeeded())
	{
		FistUltiMontage = ULTIMONTAGE4.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ULTIMONTAGE5(TEXT("/Game/Animation/Kuru_UltiMontage.Kuru_UltiMontage"));
	if (ULTIMONTAGE5.Succeeded())
	{
		NoteBookUltiMontage = ULTIMONTAGE5.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ROLLANIM(TEXT("/Game/Animation/Roll_Forward_Montage.Roll_Forward_Montage"));
	if (ROLLANIM.Succeeded())
	{
		RollActionMontage = ROLLANIM.Object;
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

			if (KRPlayerState == nullptr)
			{
				AKeroroCharacter* kero = Cast<AKeroroCharacter>(Pawn);
				if (kero)
				{
					AKeroroPlayerState* PS = kero->GetPlayerState<AKeroroPlayerState>();
					if (PS)
					{
						KRPlayerState = PS;
						UE_LOG(LogTemp, Error, TEXT("Set PS Success"));

					}
				}
			}
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
	if (IsDead || bIsHit || bIsRolling) return;

	AKeroroCharacter* kero = Cast<AKeroroCharacter>(TryGetPawnOwner());
	if (kero && KRPlayerState)
	{
		AnimationRunSpeed = KRPlayerState->AttackSpeedRate;
	}

	if (UAnimMontage* Montage = GetAnimMontage())
	{
		if (!Montage_IsPlaying(Montage))
		{
			//UE_LOG(LogTemp, Error, TEXT("AnimationRunSpeed %f"), AnimationRunSpeed);
			Montage_Play(Montage, AnimationRunSpeed);
		}
	}
}

void UKeroroAnimInstance::PlayUltiSkillMontage()
{
	if (IsDead || bIsHit || bIsRolling) return;

	if (UAnimMontage* Montage = GetAnimMontage())
	{
		if (Montage_IsPlaying(Montage))
		{
			StopAttackMontage();
		}
		if (UAnimMontage* UltiMontage = GetUltiAnimMontage())
		{
			Montage_Play(UltiMontage, 1.0f);
		}
	}
}

void UKeroroAnimInstance::PlayRollAnimation()
{
	if (IsDead || bIsHit) return;

	if (UAnimMontage* Montage = GetAnimMontage())
	{
		if (Montage_IsPlaying(Montage))
		{
			StopAttackMontage();
		}
		if (RollActionMontage)
		{
			Montage_Play(RollActionMontage, 1.0f);
		}
	}
}

void UKeroroAnimInstance::SetDeadAnim()
{
	IsDead = true;
}

void UKeroroAnimInstance::SetbIsHit(EKeroroType type)
{
	bIsHit = true;

	float AnimDuration;
	if (type == EKeroroType::Giroro) AnimDuration = 0.6f;
	else AnimDuration = 1.33f;

	FTimerHandle HitResetTimer;
	GetWorld()->GetTimerManager().SetTimer(HitResetTimer, [this]() {
		bIsHit = false;
		},
		AnimDuration,
		false
	);
}

void UKeroroAnimInstance::SetWeaponType(EWeaponType type)
{
	WeaponType = type;
}

void UKeroroAnimInstance::SetKeroroType(EKeroroType type)
{
	CurrentKeroroType = type;
}

void UKeroroAnimInstance::AnimNotify_AttackHitCheck()
{
	if (OnAttackHitCheck.IsBound())
	{
		OnAttackHitCheck.Broadcast();
	}
}

void UKeroroAnimInstance::AnimNotify_NextAttackCheck()
{
	if (OnNextAttackCheck.IsBound())
	{
		OnNextAttackCheck.Broadcast();
	}
}

void UKeroroAnimInstance::AnimNotify_EffectCreateCheck()
{
	if (OnEffectCreateCheck.IsBound())
	{
		OnEffectCreateCheck.Broadcast();
	}
}

void UKeroroAnimInstance::AnimNotify_VoiceCheck()
{
	if (OnVoiceCheck.IsBound())
	{
		OnVoiceCheck.Broadcast();
	}
}

void UKeroroAnimInstance::AnimNotify_WeaponSound()
{
	if (OnWeaponSoundCheck.IsBound())
	{
	OnWeaponSoundCheck.Broadcast();
	}
}

void UKeroroAnimInstance::AnimNotify_HitDown()
{
	bIsHit = false;
}

void UKeroroAnimInstance::AnimNotify_EndBlocking()
{
	bIsGuarding = false;
	//UE_LOG(LogTemp, Error, TEXT("HitDown"));
}

void UKeroroAnimInstance::AnimNotify_EndUltiSkill()
{
	bIsUltiSkillPlaying = false;
	//UE_LOG(LogTemp, Error, TEXT("End Ulti Skill"));
	/*auto Pawn = TryGetPawnOwner();
	AKeroroCharacter* kero = Cast<AKeroroCharacter>(Pawn);
	if (kero->CurrentKeroroType == EKeroroType::Keroro|| kero->CurrentKeroroType == EKeroroType::Kururu)
	{
		kero->ChangeCameraDefault();
	}*/
}

void UKeroroAnimInstance::AnimNotify_EndRolling()
{
	bIsRolling = false;
	//UE_LOG(LogTemp, Error, TEXT("End Rolling"));
	AKeroroCharacter* kero = Cast<AKeroroCharacter>(TryGetPawnOwner());
	if (kero)
	{
		kero->EndRoll();
	}
}

FName UKeroroAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	if (FMath::IsWithinInclusive<int32>(Section, 1, 4)) return FName(*FString::Printf(TEXT("Attack%d"), Section));
	else return NAME_None;
}

UAnimMontage* UKeroroAnimInstance::GetAnimMontage()
{
	switch (WeaponType)
	{
	case EWeaponType::FIST:
		return FistAttackMontage;
	case EWeaponType::KEROBALL:
		return KeroBallAttackMontage;
	case EWeaponType::RIFLE:
		return RifleAttackMontage;
	case EWeaponType::SWORD:
		return SwordAttackMontage;
	case EWeaponType::NOTEBOOK:
		return NoteBookAttackMontage;
	}
	return SwordAttackMontage;
}

UAnimMontage* UKeroroAnimInstance::GetUltiAnimMontage()
{
	switch (WeaponType)
	{
	case EWeaponType::FIST:
		return FistUltiMontage;
	case EWeaponType::KEROBALL:
		return KeroBallUltiMontage;
	case EWeaponType::RIFLE:
		return RifleUltiMontage;
	case EWeaponType::SWORD:
		return SwordUltiMontage;
	case EWeaponType::NOTEBOOK:
		return NoteBookUltiMontage;
	}
	return SwordUltiMontage;
}

void UKeroroAnimInstance::JumptoAttackMontageSection(int32 NewSection)
{
	if (IsDead || bIsHit || bIsRolling) return;

	if (UAnimMontage* MontageToPlay = GetAnimMontage())
	{
		if (Montage_IsPlaying(MontageToPlay))
		{
			Montage_JumpToSection(GetAttackMontageSectionName(NewSection), MontageToPlay);
		}
	}
}

void UKeroroAnimInstance::StopAttackMontage()
{
	if (UAnimMontage* Montage = GetAnimMontage())
	{
		if (Montage_IsPlaying(Montage))
		{
			Montage_Stop(0.05f, Montage);
		}
	}
}