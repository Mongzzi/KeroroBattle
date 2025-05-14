// Fill out your copyright notice in the Description page of Project Settings.


#include "KeroroWeapon.h"

// Sets default values
AKeroroWeapon::AKeroroWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SocketName = TEXT("SwordSocket");

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	RootComponent = Weapon;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WEAPON(TEXT("/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_BlackKnight/SK_Blade_BlackKnight.SK_Blade_BlackKnight"));
	if (WEAPON.Succeeded())
	{
		Weapon->SetSkeletalMesh(WEAPON.Object);
	}

	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> RIFLE(TEXT("/Game/Fab/Cartoony_Laser_Rifle/gun_part_2textured.gun_part_2textured"));
	//if (RIFLE.Succeeded())
	//{
	//	Weapon->SetSkeletalMesh(RIFLE.Object);
	//}

}

// Called when the game starts or when spawned
void AKeroroWeapon::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AKeroroWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

