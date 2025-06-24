// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordWeapon.h"
#include "KeroroCharacter.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

ASwordWeapon::ASwordWeapon()
{
	SocketNames.Add(TEXT("SwordSocket"));

	SKMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	RootComponent = SKMeshComponent;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SWORD(TEXT("/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_BlackKnight/SK_Blade_BlackKnight.SK_Blade_BlackKnight"));
	if (SWORD.Succeeded())
	{
		SKMeshComponent->SetSkeletalMesh(SWORD.Object);
		SKMeshComponent->SetCollisionProfileName(TEXT("KeroroWeapon"));
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>SWORDTRAIL(TEXT("/Game/SwordTrailVFX/VFX/NS_Trail_10.NS_Trail_10"));
	if (SWORDTRAIL.Succeeded())
	{
		NSEffect = SWORDTRAIL.Object;
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>HITEFFECT(TEXT("/Game/MixedVFX/Particles/Slashes/NS_VampireSlash.NS_VampireSlash"));
	if (HITEFFECT.Succeeded())
	{
		HitEffect = HITEFFECT.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave>SWORDSOUND13(TEXT("/Script/Engine.SoundWave'/Game/Keroro_Sound/dororo/sword_13.sword_13'"));
	if (SWORDSOUND13.Succeeded())
	{
		AttackSound.Add(SWORDSOUND13.Object);
	}

	static ConstructorHelpers::FObjectFinder<USoundWave>SWORDSOUND2(TEXT("/Script/Engine.SoundWave'/Game/Keroro_Sound/dororo/sword_2.sword_2'"));
	if (SWORDSOUND2.Succeeded())
	{
		AttackSound.Add(SWORDSOUND2.Object);
	}

	static ConstructorHelpers::FObjectFinder<USoundWave>SWORDSOUND4(TEXT("/Script/Engine.SoundWave'/Game/Keroro_Sound/dororo/sword_4.sword_4'"));
	if (SWORDSOUND2.Succeeded())
	{
		AttackSound.Add(SWORDSOUND4.Object);
	}

	static ConstructorHelpers::FObjectFinder<USoundWave>HITSOUND(TEXT("/Game/Keroro_Sound/dororo/sword_hit.sword_hit"));
	if (HITSOUND.Succeeded())
	{
		HitSound=HITSOUND.Object;
	}
}

void ASwordWeapon::PlayEffect(AKeroroCharacter* Character)
{

	if (!NSEffect || !Character)return;
	auto a = UNiagaraFunctionLibrary::SpawnSystemAttached(
		NSEffect,
		SKMeshComponent,
		TEXT("WeaponEffect"),
		FVector::ZeroVector,
		Character->GetActorRotation(),
		EAttachLocation::SnapToTargetIncludingScale,
		true
	);

	if (IsValid(a))
	{
		a->SetRelativeScale3D(FVector(1.0f));
		//a->SetAutoDestroy(true);
		//FTimerHandle TempHandle;
		//GetWorld()->GetTimerManager().SetTimer(TempHandle, [a]()
		//	{
		//		if (IsValid(a))
		//		{
		//			a->DestroyComponent();
		//		}
		//	}, 1.5f, false);
	}
}

void ASwordWeapon::PlayHitEffect(FVector HitLocation, FRotator HitRotator,FVector Scale)
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		HitEffect,
		HitLocation,
		HitRotator,
		Scale
	);
}

void ASwordWeapon::PlayHitSound(int32 CurrentCombo)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, GetActorLocation(), 1.0f);
}


// 콤보 인덱스 1~4 // 사운드 컨테이너 인덱스 0,1,2 순 저장
void ASwordWeapon::PlaySound(int32 ComboIndex)
{
	int SoundIndex;

	if (ComboIndex == 1 || ComboIndex == 3)
	{
		SoundIndex = 0;
	}
	else if (ComboIndex == 2)
	{
		SoundIndex = 1;
	}
	else {
		SoundIndex = 2;
	}

	if (AttackSound[SoundIndex])
	{
		UGameplayStatics::PlaySoundAtLocation(this, AttackSound[SoundIndex], GetActorLocation(), 1.7f);
	}
}
