// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordWeapon.h"

ASwordWeapon::ASwordWeapon()
{
	SocketName = TEXT("SwordSocket");

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	RootComponent = Weapon;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SWORD(TEXT("/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_BlackKnight/SK_Blade_BlackKnight.SK_Blade_BlackKnight"));
	if (SWORD.Succeeded())
	{
		Weapon->SetSkeletalMesh(SWORD.Object);
	}
}
