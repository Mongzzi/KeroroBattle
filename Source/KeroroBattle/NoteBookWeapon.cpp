// Fill out your copyright notice in the Description page of Project Settings.


#include "NoteBookWeapon.h"

ANoteBookWeapon::ANoteBookWeapon()
{
	SocketNames.Add(TEXT("NoteBookSocket"));
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NOTEBOOK"));
	StaticMeshComponent->SetCollisionProfileName(TEXT("KeroroWeapon"));
	RootComponent = StaticMeshComponent;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM(TEXT("/Game/Fab/Laptop_Low-poly/laptop_low_poly.laptop_low_poly"));
	if (SM.Succeeded())
	{
		StaticMeshComponent->SetStaticMesh(SM.Object);
	}

}
