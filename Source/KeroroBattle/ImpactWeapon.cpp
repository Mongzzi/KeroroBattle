// Fill out your copyright notice in the Description page of Project Settings.


#include "ImpactWeapon.h"
#include "KeroroCharacter.h"
#include "KeroroEnemyCharacter.h"
#include "KeroroStatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/DamageEvents.h"
#include "DrawDebugHelpers.h"	// 디버그 드로잉 기능 사용하기위한 헤더



AImpactWeapon::AImpactWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	UltiDuration = 3.0f;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS(TEXT("/Game/FXVarietyPack/Particles/P_ky_thunderStorm.P_ky_thunderStorm"));
	if (PS.Succeeded())
	{
		ImpactPS = PS.Object;
	}
}

void AImpactWeapon::PlayEffect(AKeroroCharacter* Character)
{
	if (!Character) return;
	OwnerKero = Character;

	if (ImpactPS)
	{
		ImpactPC = UGameplayStatics::SpawnEmitterAttached(ImpactPS, Character->GetMesh(), FName("TamamaImpact"), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true);
		ImpactPC->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.5f));
	}

	GetWorld()->GetTimerManager().SetTimer(AttackTimer, this, &AImpactWeapon::AttackCheck_Impact, 0.3f, true);

	GetWorld()->GetTimerManager().SetTimer(EffectTimer, [this]() {
		ImpactPC->DestroyComponent();
		GetWorld()->GetTimerManager().ClearTimer(AttackTimer);
		}, UltiDuration, false);
}

void AImpactWeapon::AttackCheck_Impact()
{
	if (!OwnerKero) return;
	if (!OwnerKero->KRStat) return;
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
				float Rand = FMath::FRand();
				if (Rand < OwnerKRStat->CritChanceRate)
				{
					FinalDamage *= OwnerKRStat->CritDamageRate;
				}
				FDamageEvent DamageEvent;
				HitActor->TakeDamage(FinalDamage, DamageEvent, OwnerKero->GetController(), this);
			}
		}
	}
	//DrawDebugPoint(GetWorld(), Start, 15.0f, FColor::Red, false, 1.0f);
	//DrawDebugPoint(GetWorld(), End, 15.0f, FColor::Red, false, 1.0f);

}
