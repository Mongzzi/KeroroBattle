// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordWeapon.h"

ASwordWeapon::ASwordWeapon()
{
	SocketName = TEXT("SwordSocket");

	SKMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	RootComponent = SKMeshComponent;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SWORD(TEXT("/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_BlackKnight/SK_Blade_BlackKnight.SK_Blade_BlackKnight"));
	if (SWORD.Succeeded())
	{
		SKMeshComponent->SetSkeletalMesh(SWORD.Object);
	}
}

void ASwordWeapon::PlayEffect()
{
}
