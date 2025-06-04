// Fill out your copyright notice in the Description page of Project Settings.


#include "KeroroWeapon.h"
#include "KeroroCharacter.h"

// Sets default values
AKeroroWeapon::AKeroroWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
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

void AKeroroWeapon::PlayEffect(AKeroroCharacter* Character)
{
}

void AKeroroWeapon::PlaySound(int32 ComboIndex)
{
}

void AKeroroWeapon::Throw(const FVector& Direction, float Force)
{
}

void AKeroroWeapon::ReturnToHand(AKeroroCharacter* Character)
{
}

TArray<FName> AKeroroWeapon::GetSocketNames()
{
	return SocketNames;
}

