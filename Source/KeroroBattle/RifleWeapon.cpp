// Fill out your copyright notice in the Description page of Project Settings.


#include "RifleWeapon.h"
#include "KeroroCharacter.h"
#include "Kismet/GameplayStatics.h"

ARifleWeapon::ARifleWeapon()
{
	SocketNames.Add(TEXT("RifleSocket"));

	SKMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	RootComponent = SKMeshComponent;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> RIFLE(TEXT("/Game/MuzzleFlash/Demo/FPWeapon/Mesh/SK_FPGun.SK_FPGun"));
	if (RIFLE.Succeeded())
	{
		SKMeshComponent->SetSkeletalMesh(RIFLE.Object);
		SKMeshComponent->SetCollisionProfileName(TEXT("KeroroWeapon"));

	}

	static ConstructorHelpers::FObjectFinder<USoundWave> SOUND(TEXT("/Game/MuzzleFlash/Demo/FPWeapon/Audio/FirstPersonTemplateWeaponFire02.FirstPersonTemplateWeaponFire02"));
	if (SOUND.Succeeded())
	{
		RifleFireSound = SOUND.Object;
	}

}

void ARifleWeapon::PlayEffect()
{
}

void ARifleWeapon::PlaySound(int32 ComboIndex)
{
	if (RifleFireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, RifleFireSound, GetActorLocation(),0.35f);
	}
}

void ARifleWeapon::Throw(const FVector& Direction, float Force)
{

}

void ARifleWeapon::ReturnToHand(AKeroroCharacter* Character)
{

}
