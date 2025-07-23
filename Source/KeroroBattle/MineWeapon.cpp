// Fill out your copyright notice in the Description page of Project Settings.


#include "MineWeapon.h"
#include "KeroroCharacter.h"
#include "KeroroEnemyCharacter.h"
#include "KeroroStatComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/DamageEvents.h"


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
}

void AMineWeapon::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor || OtherActor == this) return;
	if (!OwnerKero.IsValid())return;

	if (OtherActor->IsA(AKeroroEnemyCharacter::StaticClass()))
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NSEffect, GetActorLocation(), GetActorRotation());
		Destroy();

		// 데미지 처리
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

		float Damage = OwnerKero->KRStat->AttackPower;
		FDamageEvent DamageEvent;

		for (auto& Hit : HitResults)
		{
			AKeroroEnemyCharacter* Enemy = Cast<AKeroroEnemyCharacter>(Hit.GetActor());
			if (Enemy)
			{
				Enemy->TakeDamage(Damage*5, DamageEvent, OwnerKero->GetController(), OwnerKero.Get());
			}
			
		}
	}
}