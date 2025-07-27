// Fill out your copyright notice in the Description page of Project Settings.


#include "RotateWeapon.h"
#include "KeroroCharacter.h"
#include "KeroroStatComponent.h"
#include "KeroroEnemyCharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "CriticalDamageType.h"

ARotateWeapon::ARotateWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	TArray<FString> Paths = {
		TEXT("/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_SwordB/SK_Blade_SwordB.SK_Blade_SwordB"),
		TEXT("/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_DragonSword/SK_Blade_DragonSword.SK_Blade_DragonSword"),
		TEXT("/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_Fencer/SK_Blade_Fencer.SK_Blade_Fencer"),
		TEXT("/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_Glimmer/SK_Blade_Glimmer.SK_Blade_Glimmer"),
		TEXT("/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_HeroSword17/SK_Blade_HeroSword17.SK_Blade_HeroSword17")
	};
	WeaponNum = Paths.Num();

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT"));
	RootComponent = Root;

	for (int i = 0; i < WeaponNum; i++)
	{
		ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(*Paths[i]);
		if (SM.Succeeded())
		{
			FString string = TEXT("SM") + FString::FromInt(i);
			USkeletalMeshComponent* SKC = CreateDefaultSubobject<USkeletalMeshComponent>(*string);
			SKC->SetSkeletalMesh(SM.Object);
			SKC->SetRelativeScale3D(FVector(3.0f));
			SKC->SetGenerateOverlapEvents(true);
			SKC->SetCollisionProfileName(TEXT("OverlapOnlyEnemy"));
			SKC->SetupAttachment(RootComponent);
			SKC->OnComponentBeginOverlap.AddDynamic(this, &ARotateWeapon::OnWeaponOverlap);
			WeaponSKComponents.Add(SKC);
		}
	}

	// 검기 이펙트
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>SWORDTRAIL(TEXT("/Game/MixedVFX/Particles/Projectiles/NS_Shuriken_02.NS_Shuriken_02"));
	if (SWORDTRAIL.Succeeded())
	{
		NSEffect = SWORDTRAIL.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> SOUND(TEXT("/Game/Keroro_Sound/dororo/Rotating_Sword.Rotating_Sword"));
	if (SOUND.Succeeded())
	{
		UltiSound = SOUND.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> SOUND2(TEXT("/Game/Keroro_Sound/dororo/sword_hit.sword_hit"));
	if (SOUND2.Succeeded())
	{
		UltiHitSound = SOUND2.Object;
	}

	SkillDuration = 10.0f;
	CurrentAngle = 0.0f;
	RotationSpeed = 300.0f;
	RotateRadius = 300.0f;
}

void ARotateWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentAngle += RotationSpeed * DeltaTime;
	SetPositionWeapons();

}

void ARotateWeapon::SetPositionWeapons()
{
	if (OwnerKero == nullptr)
	{
		return;
	}

	FVector CenterPoint = OwnerKero->GetActorLocation();

	for (int i = 0; i < WeaponNum; i++)
	{
		// 원형 위치 계산
		float Angle = 360.0f / WeaponNum * i + CurrentAngle;
		float Radian = FMath::DegreesToRadians(Angle);

		FVector Offset = FVector(FMath::Cos(Radian), FMath::Sin(Radian), 0.0f) * RotateRadius;
		FVector Pos = CenterPoint + Offset;

		// 방향 벡터 
		FVector Dir = (Pos - CenterPoint);
		FRotator Dir_Rot = Dir.Rotation();
		Dir_Rot.Pitch -= 90.f;	// 칼이 z축으로 서있어서 눕혀줌

		WeaponSKComponents[i]->SetWorldLocation(Pos);
		WeaponSKComponents[i]->SetWorldRotation(Dir_Rot);
	}
}

void ARotateWeapon::CreateNSEffect()
{
	if (!NSEffect) return;
	for (int i = 0; i < WeaponNum; i++)
	{
		auto a = UNiagaraFunctionLibrary::SpawnSystemAttached(
			NSEffect,
			WeaponSKComponents[i],
			TEXT("WeaponEffect"),
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::SnapToTargetIncludingScale,
			true
		);
	}
}

void ARotateWeapon::PlayEffect(AKeroroCharacter* Character)
{
	OwnerKero = Character;
	CreateNSEffect();
	FTimerHandle DurationHandle;

	GetWorld()->GetTimerManager().SetTimer(DurationHandle, [this]() {
		OwnerKero->ChangeCameraDefault();
		Destroy();
		}, SkillDuration, false);
}

void ARotateWeapon::OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AKeroroEnemyCharacter* Enemy = Cast<AKeroroEnemyCharacter>(OtherActor);
	{
		AKeroroCharacter* kero = Cast<AKeroroCharacter>(GetInstigator());
		if (kero == nullptr) return;

		UKeroroStatComponent* KRStat = kero->KRStat;
		if (KRStat == nullptr) return;

		float FinalDamage = KRStat->AttackPower;
		bool bIsCritical = (FMath::FRand() < KRStat->CritChanceRate);

		FDamageEvent DamageEvent;
		if (bIsCritical)
		{
			FinalDamage *= KRStat->CritDamageRate;
			DamageEvent.DamageTypeClass = UCriticalDamageType::StaticClass();
		}

		Enemy->TakeDamage(FinalDamage, DamageEvent, GetInstigatorController(), this);

		if (UltiHitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, UltiHitSound, GetActorLocation(), 0.5f);
		}
	}
}

void ARotateWeapon::BeginPlay()
{
	Super::BeginPlay();
	if (UltiSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, UltiSound, GetActorLocation(), 0.5f);
	}
}
