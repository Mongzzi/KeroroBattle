// Fill out your copyright notice in the Description page of Project Settings.


#include "RifleWeapon.h"


ARifleWeapon::ARifleWeapon()
{
	SocketName = TEXT("RifleSocket");

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	RootComponent = Weapon;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> RIFLE(TEXT("/Game/Fab/Cartoony_Laser_Rifle/gun_part_2textured.gun_part_2textured"));
	if (RIFLE.Succeeded())
	{
		Weapon->SetSkeletalMesh(RIFLE.Object);
	}
}
