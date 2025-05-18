// Fill out your copyright notice in the Description page of Project Settings.


#include "KeroballWeapon.h"
#include "KeroroCharacter.h"

AKeroballWeapon::AKeroballWeapon()
{
	SocketName = TEXT("KeroballSocket");

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPON"));
	StaticMeshComponent->SetCollisionProfileName(TEXT("KeroroWeapon"));
	RootComponent = StaticMeshComponent;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> KB(TEXT("/Game/Keroro_Model/etc/keroball.keroball"));
	if (KB.Succeeded())
	{
		StaticMeshComponent->SetStaticMesh(KB.Object);
		StaticMeshComponent->SetSimulatePhysics(false);
	}
}

void AKeroballWeapon::Throw(const FVector& Direction, float Force)
{
	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->AddImpulse(Direction * Force, NAME_None, true);
}

void AKeroballWeapon::ReturnToHand(AKeroroCharacter* Character)
{
	if (!Character) return;
	StaticMeshComponent->SetSimulatePhysics(false);
	AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
}

