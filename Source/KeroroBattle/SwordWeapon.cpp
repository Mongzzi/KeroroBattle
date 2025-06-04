// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordWeapon.h"
#include "KeroroCharacter.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

ASwordWeapon::ASwordWeapon()
{
	SocketNames.Add(TEXT("SwordSocket"));

	SKMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	RootComponent = SKMeshComponent;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SWORD(TEXT("/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_BlackKnight/SK_Blade_BlackKnight.SK_Blade_BlackKnight"));
	if (SWORD.Succeeded())
	{
		SKMeshComponent->SetSkeletalMesh(SWORD.Object);
		SKMeshComponent->SetCollisionProfileName(TEXT("KeroroWeapon"));
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>SWORDTRAIL(TEXT("/Game/SwordTrailVFX/VFX/NS_Trail_10.NS_Trail_10"));
	if (SWORDTRAIL.Succeeded())
	{
		NSEffect = SWORDTRAIL.Object;
	}
} 

void ASwordWeapon::PlayEffect(AKeroroCharacter* Character)
{

	if (!NSEffect || !Character)return;
	auto a = UNiagaraFunctionLibrary::SpawnSystemAttached(
		NSEffect,
		SKMeshComponent,
		TEXT("WeaponEffect"),
		FVector::ZeroVector,
		Character->GetActorRotation(),
		EAttachLocation::SnapToTargetIncludingScale,
		true
	);

	if (IsValid(a))
	{
		a->SetRelativeScale3D(FVector(1.0f));
		//a->SetAutoDestroy(true);
		//FTimerHandle TempHandle;
		//GetWorld()->GetTimerManager().SetTimer(TempHandle, [a]()
		//	{
		//		if (IsValid(a))
		//		{
		//			a->DestroyComponent();
		//		}
		//	}, 1.5f, false);
	}
}
