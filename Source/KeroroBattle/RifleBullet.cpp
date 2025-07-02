// Fill out your copyright notice in the Description page of Project Settings.


#include "RifleBullet.h"
#include "KeroroEnemyCharacter.h"
#include "KeroroCharacter.h"
#include "KeroroStatComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"


ARifleBullet::ARifleBullet()
{
	PrimaryActorTick.bCanEverTick = false;

	// 콜리전
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("COLLISION"));
	Collision->InitSphereRadius(10.0f);
	Collision->SetCollisionProfileName(TEXT("Bullet"));
	RootComponent = Collision;
	Collision->OnComponentHit.AddDynamic(this, &ARifleBullet::OnHit);

	// 메시
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("/Game/Fab/Dirt_Bullet_caliber_9x19/dirt_bullet_caliber_9x19.dirt_bullet_caliber_9x19"));
	if (MeshAsset.Succeeded())
	{
		Mesh->SetStaticMesh(MeshAsset.Object);
		Mesh->SetRelativeScale3D(FVector(10.0f));
	}

	// 나이아가라 시스템 
	BulletEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("BulletEffect"));
	BulletEffectComponent->SetupAttachment(Mesh);

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>BULLET(TEXT("/Game/MixedVFX/Particles/Projectiles/NS_Projectile_01.NS_Projectile_01"));
	if (BULLET.Succeeded())
	{
		BulletEffect = BULLET.Object;
		BulletEffectComponent->SetAsset(BulletEffect);
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>HITEFFECT(TEXT("/Game/MixedVFX/Particles/Projectiles/Hits/NS_GalaxyArrow_Hit.NS_GalaxyArrow_Hit"));
	if (HITEFFECT.Succeeded())
	{
		HitEffect = HITEFFECT.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave>HITSOUND(TEXT("/Game/Keroro_Sound/giroro/bullet_hit1.bullet_hit1"));
	if (HITSOUND.Succeeded())
	{
		HitSounds.Add(HITSOUND.Object);
	}

	static ConstructorHelpers::FObjectFinder<USoundWave>HITSOUND2(TEXT("/Game/Keroro_Sound/giroro/bullet_hit2.bullet_hit2"));
	if (HITSOUND2.Succeeded())
	{
		HitSounds.Add(HITSOUND2.Object);
	}

	// 발사체 이동 컴포넌트
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("PROJECTILE"));
	ProjectileMovement->InitialSpeed = BulletSpeed;
	ProjectileMovement->MaxSpeed = BulletSpeed;
	ProjectileMovement->ProjectileGravityScale = 0.0f; // 중력 무시
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	InitialLifeSpan = LifeSpanTime;
}

void ARifleBullet::BeginPlay()
{
	Super::BeginPlay();
}

void ARifleBullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && Cast<AKeroroEnemyCharacter>(OtherActor))
	{
		AKeroroCharacter* kero = Cast<AKeroroCharacter>(GetInstigator());
		if (kero == nullptr) return;

		UKeroroStatComponent* kero_stat = kero->KRStat;
		if (kero_stat == nullptr) return;

		float FinalDamage = kero_stat->SetFinalDamage();


		UGameplayStatics::ApplyDamage(OtherActor, FinalDamage, GetInstigatorController(), this, nullptr);

		if (HitEffect)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				HitEffect,
				Hit.ImpactPoint,
				Hit.ImpactNormal.Rotation(),
				FVector(10.0f)
			);
		}

		if (HitSounds[0] && HitSounds[1])
		{
			int RandInt = FMath::RandRange(0, 1);
			UGameplayStatics::PlaySoundAtLocation(this, HitSounds[RandInt], GetActorLocation(), 1.0f);
		}

	}
	Destroy();
}

