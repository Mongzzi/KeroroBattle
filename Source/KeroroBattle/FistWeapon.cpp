// Fill out your copyright notice in the Description page of Project Settings.


#include "FistWeapon.h"
#include "KeroroCharacter.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

AFistWeapon::AFistWeapon()
{
	SocketNames.Add(TEXT("RightFistSocket"));
	SocketNames.Add(TEXT("LeftFistSocket"));
	//static ConstructorHelpers::FObjectFinder<UNiagaraSystem>FIST(TEXT("/Game/MixedVFX/Particles/Fires/NS_Campfire.NS_Campfire"));
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>FIST(TEXT("/Game/MixedVFX/Particles/Projectiles/NS_Projectile_04.NS_Projectile_04"));
	if (FIST.Succeeded())
	{
		NSEffect = FIST.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave>SOUND1(TEXT("/Game/keroro_asset/punch.punch"));
	if (SOUND1.Succeeded())
	{
		FistComboAttackSounds.Add(SOUND1.Object);
	}
	static ConstructorHelpers::FObjectFinder<USoundWave>SOUND2(TEXT("/Game/keroro_asset/punch.punch"));
	if (SOUND2.Succeeded())
	{
		FistComboAttackSounds.Add(SOUND2.Object);
	}
	static ConstructorHelpers::FObjectFinder<USoundWave>SOUND3(TEXT("/Game/keroro_asset/punch.punch"));
	if (SOUND3.Succeeded())
	{
		FistComboAttackSounds.Add(SOUND3.Object);
	}
	static ConstructorHelpers::FObjectFinder<USoundWave>SOUND4(TEXT("/Game/keroro_asset/punch.punch"));
	if (SOUND4.Succeeded())
	{
		FistComboAttackSounds.Add(SOUND4.Object);
	}
}

void AFistWeapon::InitEffect(AKeroroCharacter* kero)
{
	if (!kero || !NSEffect) return;

	USkeletalMeshComponent* Mesh = kero->GetMesh();

	// ¿À¸¥¼Õ ÀÌÆåÆ®
	auto a = UNiagaraFunctionLibrary::SpawnSystemAttached(
		NSEffect,
		Mesh,
		TEXT("RightFistSocket"),
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::SnapToTargetIncludingScale,
		true
	);
	if (IsValid(a))
	{
		a->SetRelativeScale3D(FVector(0.5f));
		NCEffects.Add(a);
	}

	// ¿Þ¼Õ ÀÌÆåÆ®
	auto b = UNiagaraFunctionLibrary::SpawnSystemAttached(
		NSEffect,
		Mesh,
		TEXT("LeftFistSocket"),
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::SnapToTargetIncludingScale,
		true
	);
	if (IsValid(b))
	{
		b->SetRelativeScale3D(FVector(0.5f));
		NCEffects.Add(b);
	}
}

void AFistWeapon::PlaySound(int32 ComboIndex)
{
	if (FMath::IsWithinInclusive<int32>(ComboIndex, 1, 4))
	{
		UGameplayStatics::PlaySoundAtLocation(this, FistComboAttackSounds[ComboIndex - 1], GetActorLocation());
	}
}

void AFistWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AFistWeapon::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	for (UNiagaraComponent* a : NCEffects)
	{
		if (IsValid(a))
		{
			a->DestroyComponent();
		}
	}
}
