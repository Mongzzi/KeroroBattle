// Fill out your copyright notice in the Description page of Project Settings.


#include "FistWeapon.h"
#include "KeroroCharacter.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

AFistWeapon::AFistWeapon()
{
	SocketNames.Add(TEXT("RightFistSocket"));
	SocketNames.Add(TEXT("LeftFistSocket"));

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>FIST(TEXT("/Game/MixedVFX/Particles/Fires/NS_Campfire.NS_Campfire"));
	if (FIST.Succeeded())
	{
		NSEffect = FIST.Object;
	}
}

void AFistWeapon::InitEffect(AKeroroCharacter* kero)
{
	if (!kero || !NSEffect) return;

	USkeletalMeshComponent* Mesh = kero->GetMesh();

	// ø¿∏•º’ ¿Ã∆Â∆Æ
	UNiagaraFunctionLibrary::SpawnSystemAttached(
		NSEffect,
		Mesh,
		TEXT("RightFistSocket"),
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::SnapToTargetIncludingScale,
		true
	);

	// øﬁº’ ¿Ã∆Â∆Æ
	UNiagaraFunctionLibrary::SpawnSystemAttached(
		NSEffect,
		Mesh,
		TEXT("LeftFistSocket"),
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::SnapToTargetIncludingScale,
		true
	);
}

void AFistWeapon::BeginPlay()
{
	Super::BeginPlay();
}
