// Fill out your copyright notice in the Description page of Project Settings.


#include "KeroballWeapon.h"

AKeroballWeapon::AKeroballWeapon()
{
	SocketName = TEXT("KeroballSocket");

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPON"));
	RootComponent = SKMeshComponent;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> KB(TEXT("/Game/Keroro_Model/etc/keroball.keroball"));
	if (KB.Succeeded())
	{
		StaticMeshComponent->SetStaticMesh(KB.Object);
		StaticMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
	}
}