// Fill out your copyright notice in the Description page of Project Settings.


#include "ViperCharacter.h"
#include "KeroroAnimInstance.h"
#include "KeroroStatComponent.h"
#include "Engine/DamageEvents.h"
#include "CriticalDamageType.h"
#include "KeroroCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"



AViperCharacter::AViperCharacter(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	EnemyType = EEnemyType::Viper;
	GetCapsuleComponent()->SetWorldScale3D(FVector(2.5f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SKMESH(TEXT("/Game/Keroro_Model/Viper/Viper.Viper"));
	if (SKMESH.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SKMESH.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance>ANIM(TEXT("/Game/Keroro_Model/Viper/ViperAnimInstance.ViperAnimInstance_C"));
	if (ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(ANIM.Class);
	}
	AttackRange = 300.0f;
	AttackRadius = 300.0f;
	MaxWalkSpeed = 1200.0f;
}

void AViperCharacter::Attack()
{
	if (!EnemyAnim) return;
	EnemyAnim->PlayEnemyAttackMontage(EnemyType);
}

void AViperCharacter::AttackCheck()
{
	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		GetActorLocation(),
		GetActorLocation(),
		FQuat::Identity,
		ECC_GameTraceChannel3,
		FCollisionShape::MakeSphere(AttackRadius),
		Params
	);

	if (bHit)
	{
		for (const FHitResult& Hit : HitResults)
		{
			AActor* HitActor = Hit.GetActor();
			if (IsValid(HitActor) && Cast<AKeroroCharacter>(Hit.GetActor()))
			{
				float FinalDamage = EnemyStat->AttackPower;
				bool bIsCritical = (FMath::FRand() < EnemyStat->CritChanceRate);

				FDamageEvent DamageEvent;
				if (bIsCritical)
				{
					FinalDamage *= EnemyStat->CritDamageRate;
					DamageEvent.DamageTypeClass = UCriticalDamageType::StaticClass();
				}

				HitActor->TakeDamage(FinalDamage * 3, DamageEvent, GetController(), this);

			}
		}
	}
}
