// Fill out your copyright notice in the Description page of Project Settings.


#include "RoboboCharacter.h"
#include "KeroroAnimInstance.h"
#include "KeroroStatComponent.h"
#include "CriticalDamageType.h"
#include "KeroroCharacter.h"
#include "Engine/DamageEvents.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"


ARoboboCharacter::ARoboboCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	EnemyType = EEnemyType::Robobo;
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SKMESH(TEXT("/Game/Keroro_Model/robobo/Robobo.Robobo"));
	if (SKMESH.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SKMESH.Object);
	}
	AttackRange = 450.0f;
	AttackRadius = 300.0f;

	static ConstructorHelpers::FObjectFinder<UParticleSystem>EFFECT(TEXT("/Game/FXVarietyPack/Particles/P_ky_waterBallHit.P_ky_waterBallHit"));
	if (EFFECT.Succeeded())
	{
		ParticleSystem = EFFECT.Object;
	}

}

void ARoboboCharacter::Attack()
{
	if (!EnemyAnim) return;
	EnemyAnim->PlayEnemyAttackMontage(EnemyType);
}

void ARoboboCharacter::AttackCheck()
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

				HitActor->TakeDamage(FinalDamage * 2, DamageEvent, GetController(), this);
				
				if (ParticleSystem)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleSystem, HitActor->GetActorLocation(), FRotator::ZeroRotator, FVector(0.3),true);
				}
			}
		}
	}
}
