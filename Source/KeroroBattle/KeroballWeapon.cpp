// Fill out your copyright notice in the Description page of Project Settings.


#include "KeroballWeapon.h"
#include "KeroroCharacter.h"
#include "KeroroEnemyCharacter.h"
#include "KeroroStatComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include "CriticalDamageType.h"

void AKeroballWeapon::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (StaticMeshComponent)
	{
		StaticMeshComponent->OnComponentHit.AddDynamic(this, &AKeroballWeapon::OnHit);
	}
	Super::EndPlay(EndPlayReason);
}

AKeroballWeapon::AKeroballWeapon()
{
	SocketNames.Add(TEXT("KeroballSocket"));

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPON"));
	StaticMeshComponent->SetCollisionProfileName(TEXT("KeroroWeapon"));
	RootComponent = StaticMeshComponent;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> KB(TEXT("/Game/Keroro_Model/etc/keroball/keroball.keroball"));
	if (KB.Succeeded())
	{
		StaticMeshComponent->SetStaticMesh(KB.Object);
		StaticMeshComponent->SetSimulatePhysics(false);
		StaticMeshComponent->SetNotifyRigidBodyCollision(true);
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>NS(TEXT("/Game/MixedVFX/Particles/Explosions/NS_ExplosionGroundSmall.NS_ExplosionGroundSmall"));
	if (NS.Succeeded())
	{
		NSEffect = NS.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> SOUND(TEXT("/Game/Keroro_Sound/weapon/keroball/bomb.bomb"));
	if (SOUND.Succeeded())
	{
		BombSound = SOUND.Object;
	}

	StaticMeshComponent->OnComponentHit.AddDynamic(this, &AKeroballWeapon::OnHit);
	bIsThrowing = false;
}

void AKeroballWeapon::Throw(const FVector& Direction, float Force)
{
	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->AddImpulse(Direction * Force, NAME_None, true);
	bIsThrowing = true;
	GetWorldTimerManager().SetTimer(ExplodeTimerHandle, this, &AKeroballWeapon::Explode, BombTime, false);
}

void AKeroballWeapon::ReturnToHand(AKeroroCharacter* Character)
{
	if (!Character) return;
	if (!SocketNames.IsValidIndex(0))return;
	StaticMeshComponent->SetSimulatePhysics(false);
	AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, SocketNames[0]);
}

void AKeroballWeapon::Explode()
{
	if (!IsValid(this)) return;

	//UE_LOG(LogTemp, Warning, TEXT("Explode called"));


	TArray<FHitResult> HitResults;
	float DamageRadius = 300.0f;

	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		GetActorLocation(),
		GetActorLocation(),
		FQuat::Identity,
		ECC_GameTraceChannel3,
		FCollisionShape::MakeSphere(DamageRadius)
	);

	if (bHit)
	{
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
			AActor* HitActor = Hit.GetActor();
			if (HitActor && HitActor->IsA(AKeroroEnemyCharacter::StaticClass()))
			{
				HitActor->TakeDamage(FinalDamage * 3, DamageEvent, GetInstigatorController(), this);
				GetWorldTimerManager().ClearTimer(ExplodeTimerHandle);
			}
		}
	}
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NSEffect, GetActorLocation(), FRotator::ZeroRotator, FVector(3.0f));
	if (BombSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, BombSound, GetActorLocation(), BombSoundVolume);
	}
	Destroy();
}

void AKeroballWeapon::FallDown()
{
	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->AddImpulse(FVector(0.0f, 0.0f, -100.0f), NAME_None, true);
	BombSoundVolume = 0.5f;
	GetWorldTimerManager().SetTimer(ExplodeTimerHandle, this, &AKeroballWeapon::Explode, 3.0f, false);
}

void AKeroballWeapon::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor || OtherActor == this)
		return;

	if (OtherActor->IsA(AKeroroEnemyCharacter::StaticClass()))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Attack succed"));
		Explode();
	}
}

