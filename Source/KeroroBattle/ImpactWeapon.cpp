// Fill out your copyright notice in the Description page of Project Settings.


#include "ImpactWeapon.h"
#include "KeroroCharacter.h"
#include "KeroroEnemyCharacter.h"
#include "KeroroStatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "CriticalDamageType.h"


void AImpactWeapon::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetTimerManager().ClearTimer(AttackTimer);
	Super::EndPlay(EndPlayReason);

}

AImpactWeapon::AImpactWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	UltiDuration = 3.0f;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS(TEXT("/Game/FXVarietyPack/Particles/P_ky_thunderStorm.P_ky_thunderStorm"));
	if (PS.Succeeded())
	{
		ImpactPS = PS.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> SOUND2(TEXT("/Game/Keroro_Sound/tamama/Tama_Ulti_Hit_Sound.Tama_Ulti_Hit_Sound"));
	if (SOUND2.Succeeded())
	{
		UltiHitSound = SOUND2.Object;
	}
}

void AImpactWeapon::PlayEffect(AKeroroCharacter* Character)
{
	if (!Character) return;
	OwnerKero = Character;

	if (ImpactPS)
	{
		ImpactPC = UGameplayStatics::SpawnEmitterAttached(ImpactPS, Character->GetMesh(), FName("TamamaImpact"), FVector::ZeroVector, FRotator(0.0f, 0.0f, -15.0f), EAttachLocation::SnapToTarget, true);
		ImpactPC->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.5f));
	}

	TWeakObjectPtr<AImpactWeapon> WeakThis(this);

	GetWorld()->GetTimerManager().SetTimer(AttackTimer, this, &AImpactWeapon::AttackCheck_Impact, 0.1f, true);

	GetWorld()->GetTimerManager().SetTimer(EffectTimer, [WeakThis]() {
		if (!WeakThis.IsValid())return;
		if (WeakThis->OwnerKero.IsValid())
		{
			WeakThis->OwnerKero->ChangeCameraDefault();
		}
		if (WeakThis->ImpactPC)
		{
			WeakThis->ImpactPC->DestroyComponent();
		}
		WeakThis->GetWorld()->GetTimerManager().ClearTimer(WeakThis->AttackTimer);
		}, UltiDuration, false);
}

void AImpactWeapon::AttackCheck_Impact()
{
	if (!OwnerKero.IsValid() || !OwnerKero->KRStat) return;
	UKeroroStatComponent* OwnerKRStat = OwnerKero->KRStat;

	FVector Start = OwnerKero->GetMesh()->GetSocketLocation("TamamaImpact");
	FVector Forward = OwnerKero->GetActorForwardVector();
	FVector End = Start + Forward * 1500.0f;

	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		Start,
		End,
		FQuat::Identity,
		ECC_GameTraceChannel3,
		FCollisionShape::MakeSphere(300.0f)
	);

	if (bHit)
	{
		for (const FHitResult& Hit : HitResults)
		{
			AActor* HitActor = Hit.GetActor();
			if (IsValid(HitActor) && Cast<AKeroroEnemyCharacter>(Hit.GetActor()))
			{

				float FinalDamage = OwnerKRStat->AttackPower;
				bool bIsCritical = (FMath::FRand() < OwnerKRStat->CritChanceRate);

				FDamageEvent DamageEvent;
				if (bIsCritical)
				{
					FinalDamage *= OwnerKRStat->CritDamageRate;
					DamageEvent.DamageTypeClass = UCriticalDamageType::StaticClass();
				}
				HitActor->TakeDamage(FinalDamage, DamageEvent, OwnerKero->GetController(), GetInstigator());
			}
		}
		if (UltiHitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, UltiHitSound, GetActorLocation(), 0.6f);
		}
	}
}
