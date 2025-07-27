// Fill out your copyright notice in the Description page of Project Settings.


#include "MineWeapon.h"
#include "KeroroCharacter.h"
#include "KeroroEnemyCharacter.h"
#include "KeroroStatComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "CriticalDamageType.h"

AMineWeapon::AMineWeapon()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MINE"));
	RootComponent = StaticMeshComponent;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM(TEXT("/Game/Fab/Landmine/landmine.landmine"));
	if (SM.Succeeded())
	{
		StaticMeshComponent->SetStaticMesh(SM.Object);
		StaticMeshComponent->SetCollisionProfileName(TEXT("KeroroWeapon"));
		StaticMeshComponent->SetSimulatePhysics(true);
		StaticMeshComponent->OnComponentHit.AddDynamic(this, &AMineWeapon::OnHit);
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NS(TEXT("/Game/MixedVFX/Particles/Explosions/NS_ExplosionGroundBig.NS_ExplosionGroundBig"));
	if (NS.Succeeded())
	{
		NSEffect = NS.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> SOUND2(TEXT("/Game/Keroro_Sound/weapon/keroball/bomb2.bomb2"));
	if (SOUND2.Succeeded())
	{
		UltiHitSound = SOUND2.Object;
	}
}

void AMineWeapon::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor || OtherActor == this) return;


	if (OtherActor->IsA(AKeroroEnemyCharacter::StaticClass()))
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NSEffect, GetActorLocation(), GetActorRotation());

		TArray<FHitResult> HitResults;
		float DamageRadius = 300.0f;

		bool bHit = GetWorld()->SweepMultiByChannel(
			HitResults,
			GetActorLocation(),
			GetActorLocation(),
			FQuat::Identity,
			ECC_EngineTraceChannel3,
			FCollisionShape::MakeSphere(DamageRadius)
		);


		AKeroroCharacter* kero = Cast<AKeroroCharacter>(GetInstigator());
		if (kero == nullptr) return;

		UKeroroStatComponent* KRStat = kero->KRStat;
		if (KRStat == nullptr) return;

		float FinalDamage = KRStat->AttackPower;
		bool bIsCritical = (FMath::FRand() < KRStat->CritChanceRate);

		FDamageEvent DamageEvent;
		if (bIsCritical)
		{
			FinalDamage *= KRStat->CritDamageRate;
			DamageEvent.DamageTypeClass = UCriticalDamageType::StaticClass();
		}

		for (auto& Hit : HitResults)
		{
			AKeroroEnemyCharacter* Enemy = Cast<AKeroroEnemyCharacter>(Hit.GetActor());
			if (Enemy)
			{
				Enemy->TakeDamage(FinalDamage*5, DamageEvent,GetInstigatorController(), GetInstigator());
			}
			
		}
		if (UltiHitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, UltiHitSound, GetActorLocation(), 3.0f);
		}
		Destroy();
	}
}