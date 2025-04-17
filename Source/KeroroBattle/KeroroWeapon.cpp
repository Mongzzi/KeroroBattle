// Fill out your copyright notice in the Description page of Project Settings.


#include "KeroroWeapon.h"

// Sets default values
AKeroroWeapon::AKeroroWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	RootComponent = Weapon;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WEAPON(TEXT("/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_BlackKnight/SK_Blade_BlackKnight.SK_Blade_BlackKnight"));
	if (WEAPON.Succeeded())
	{
		Weapon->SetSkeletalMesh(WEAPON.Object);
	}

	AttachLocationOffset = FVector(0.684671, -10.78493, 3.753229);
	AttachRotationOffset = FRotator(-179.845136, -90.0, 179.845127);
	AttachScale = FVector(1.5f, 1.5f, 1.5f);
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

