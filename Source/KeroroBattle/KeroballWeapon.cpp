// Fill out your copyright notice in the Description page of Project Settings.


#include "KeroballWeapon.h"
#include "KeroroCharacter.h"
#include "KeroroEnemyCharacter.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

AKeroballWeapon::AKeroballWeapon()
{
	SocketNames.Add(TEXT("KeroballSocket"));

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPON"));
	StaticMeshComponent->SetCollisionProfileName(TEXT("KeroroWeapon"));
	RootComponent = StaticMeshComponent;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> KB(TEXT("/Game/Keroro_Model/etc/keroball.keroball"));
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

	static ConstructorHelpers::FObjectFinder<USoundWave> SOUND(TEXT("/Game/keroro_asset/bomb.bomb"));
	if (SOUND.Succeeded())
	{
		BombSound = SOUND.Object;
	}

	StaticMeshComponent->OnComponentHit.AddDynamic(this, &AKeroballWeapon::OnHit);
}

void AKeroballWeapon::Throw(const FVector& Direction, float Force)
{
	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->AddImpulse(Direction * Force, NAME_None, true);
	GetWorldTimerManager().SetTimer(ExplodeTimerHandle, this, &AKeroballWeapon::Explode, 1.25f, false);
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
	float DamageAmount = 300.0f;

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
		for (auto& Hit : HitResults)
		{
			AActor* HitActor = Hit.GetActor();
			if (HitActor && HitActor->IsA(AKeroroEnemyCharacter::StaticClass()))
			{
				UGameplayStatics::ApplyDamage(HitActor, DamageAmount, GetInstigatorController(), this, UDamageType::StaticClass());
				GetWorldTimerManager().ClearTimer(ExplodeTimerHandle);
			}
		}
	}
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NSEffect, GetActorLocation(), FRotator::ZeroRotator, FVector(3.0f));
	if (BombSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, BombSound, GetActorLocation(),3.0f);
	}
	Destroy();
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

