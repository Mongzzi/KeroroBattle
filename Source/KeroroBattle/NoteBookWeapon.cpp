// Fill out your copyright notice in the Description page of Project Settings.


#include "NoteBookWeapon.h"
#include "KeroroCharacter.h"
#include "KeroroStatComponent.h"
#include "KeroroEnemyCharacter.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Engine/DamageEvents.h"
#include "CriticalDamageType.h"


ANoteBookWeapon::ANoteBookWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	// 노트북 메시
	SocketNames.Add(TEXT("NoteBookSocket"));

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NOTEBOOK"));
	StaticMeshComponent->SetCollisionProfileName(TEXT("KeroroWeapon"));
	RootComponent = StaticMeshComponent;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM(TEXT("/Game/Fab/Laptop_Low-poly/laptop_low_poly.laptop_low_poly"));
	if (SM.Succeeded())
	{
		StaticMeshComponent->SetStaticMesh(SM.Object);
	}

	// 스킬 이펙트
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NS_Magic(TEXT("/Game/Vefects/Zap_VFX/VFX/Zap/Particles/NS_Zap_01_Red.NS_Zap_01_Red"));
	if (NS_Magic.Succeeded())
	{
		MagicCircleEffect = NS_Magic.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> NS_Magic2(TEXT("/Game/FXVarietyPack/Particles/P_ky_magicCircle1.P_ky_magicCircle1"));
	if (NS_Magic2.Succeeded())
	{
		MagicCircleEffect2 = NS_Magic2.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> NS_Magic3(TEXT("/Game/FXVarietyPack/Particles/P_ky_fireStorm.P_ky_fireStorm"));
	if (NS_Magic3.Succeeded())
	{
		MagicCircleEffect3 = NS_Magic3.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> SOUND(TEXT("/Game/Keroro_Sound/weapon/notebook/FireWallSound.FireWallSound"));
	if (SOUND.Succeeded())
	{
		FinalEffectSound = SOUND.Object;
	}


	FinalEffectLoc = FVector::ZeroVector;
}

void ANoteBookWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ANoteBookWeapon::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ANoteBookWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANoteBookWeapon::PlayHitSound(int32 CurrentCombo)
{
}

void ANoteBookWeapon::PlaySound(int32 ComboIndex)
{
	if (!FinalEffectSound) return;
	UAudioComponent* AudioComp = UGameplayStatics::SpawnSoundAtLocation(this, FinalEffectSound, GetActorLocation(), FRotator::ZeroRotator, 1.0f);

	if (!AudioComp) return;
	TWeakObjectPtr<UAudioComponent> WeakAudioComp(AudioComp);

	FTimerHandle TimeHandle;
	GetWorld()->GetTimerManager().SetTimer(TimeHandle, [WeakAudioComp](){
			if (WeakAudioComp.IsValid() && WeakAudioComp->IsPlaying())
			{
				WeakAudioComp->Stop();
			}
		}, EffectRemainTime, false);
}

void ANoteBookWeapon::ActivateMagicCircle()
{
	if (!MagicCircleEffect || !OwnerKero.IsValid()) return;

	APlayerController* PC = Cast<APlayerController>(OwnerKero->GetController());
	if (!PC) return;

	FVector WorldLocation, WorldDirection;
	if (!PC->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
		return;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerKero.Get());

	// 첫번째 라인트레이스 
	FVector Start = WorldLocation;
	FVector End = Start + WorldDirection * TraceDistance;

	FHitResult HitResult;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
	{
		// 바닥인 경우 - 내적값 1에 가까울수록 두 벡터가 수평에 가까움 a,b내적 = 스칼라 a,b곱에 코사인 세타값 따라서 세타값이 0에가까울수록 내적값은 1에 가까움
		float Dot = FVector::DotProduct(HitResult.ImpactNormal, FVector::UpVector);
		if (Dot > 0.9f)
		{
			FinalEffectLoc = HitResult.Location;
			SpawnOrUpdateEffect_Particle(HitResult.Location, FRotator::ZeroRotator);
			return;
		}
	}

	// 첫번쨰 라인트레이스에서 충돌이없는경우 컨트롤러의 방향에서 yaw값만 가져와 forward 벡터가져오고
	// 끝부분인 TraceDistance 거리에서 업벡터의 음수방향으로 라인트레이스를 쏴 바닥과 충돌하는 지점 찾아서 해당위치에 이펙트
	FRotator Rotation = FRotator(0.0f, PC->GetControlRotation().Yaw, 0.0f);
	FVector Forward = Rotation.Vector();
	End = Start + Forward * TraceDistance;

	FHitResult DownHit;
	FVector DownTraceStart = End;
	FVector DownTraceEnd = DownTraceStart + FVector(0.0f, 0.0f, -10000.f);

	if (GetWorld()->LineTraceSingleByChannel(DownHit, DownTraceStart, DownTraceEnd, ECC_Visibility, Params))
	{
		FinalEffectLoc = HitResult.Location;
		SpawnOrUpdateEffect_Particle(DownHit.Location, FRotator::ZeroRotator); // 바닥 위에 정렬
	}
}

void ANoteBookWeapon::ActivateFinalEffect()
{
	if (!OwnerKero.IsValid()) return;

	PlaySound(0);

	FVector loc = FinalEffectLoc;

	TWeakObjectPtr<UParticleSystemComponent> TempEffect = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),MagicCircleEffect3,loc,FRotator::ZeroRotator,FVector(1.0f));

	FTimerHandle EffectTimerHandle;
	TWeakObjectPtr<UParticleSystemComponent> WeakTempEffect = TempEffect;

	GetWorld()->GetTimerManager().SetTimer(EffectTimerHandle, [WeakTempEffect]() {
		if (WeakTempEffect.IsValid()){
			WeakTempEffect->DestroyComponent();
		}}, EffectRemainTime, false);

	TWeakObjectPtr<ANoteBookWeapon> WeakThis(this);

	FTimerHandle DamageTickHandle;
	GetWorld()->GetTimerManager().SetTimer(DamageTickHandle,[WeakThis, loc]() {
			if (WeakThis.IsValid())
			{
				WeakThis->AttackCheck_NoteBook(loc);
			}},DamageTickInterval,true);

	FTimerHandle StopHandle;
	GetWorld()->GetTimerManager().SetTimer(StopHandle,[WeakThis, DamageTickHandle]() mutable{
			if (WeakThis.IsValid())
			{
				WeakThis->GetWorld()->GetTimerManager().ClearTimer(DamageTickHandle);
			}},EffectRemainTime,false);
}


void ANoteBookWeapon::AttackCheck_NoteBook(FVector Location)
{
	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		Location,
		Location + FVector::UpVector * 700.0f,
		FQuat::Identity,
		ECC_GameTraceChannel3,
		FCollisionShape::MakeSphere(AttackRadius),
		Params
	);

	if (bHit)
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

		for (const FHitResult& Hit : HitResults)
		{
			AActor* HitActor = Hit.GetActor();
			if (IsValid(HitActor) && Cast<AKeroroEnemyCharacter>(Hit.GetActor()))
			{
				HitActor->TakeDamage(FinalDamage / 3, DamageEvent, OwnerKero->GetController(), GetInstigator());
			}
		}
	}
}

void ANoteBookWeapon::DeactivateMagicCircle()
{
	if (MagicCircleNiagara)
	{
		MagicCircleNiagara->Deactivate();
	}
	bIsMagicCircleActive = false;
}

void ANoteBookWeapon::DeactivateMagicCircle2()
{
	if (MagicCircleParticle)
	{
		MagicCircleParticle->DeactivateSystem();
	}
	bIsMagicCircleActive = false;
}

void ANoteBookWeapon::SpawnOrUpdateEffect(FVector Location, FRotator Rotation)
{
	if (!MagicCircleNiagara)
	{
		UE_LOG(LogTemp, Error, TEXT("effect create"));
		MagicCircleNiagara = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			MagicCircleEffect,
			Location,
			Rotation,
			FVector(10.0f),
			false
		);
	}
	else
	{
		MagicCircleNiagara->SetWorldLocation(Location);
		MagicCircleNiagara->SetWorldRotation(Rotation);
		if (!bIsMagicCircleActive)
		{
			MagicCircleNiagara->Activate(true);
			bIsMagicCircleActive = true;
		}
	}

	bIsMagicCircleActive = true;
	//DrawDebugSphere(GetWorld(), Location, 50.f, 12, FColor::Blue, false, 2.f);
}


void ANoteBookWeapon::SpawnOrUpdateEffect_Particle(FVector Location, FRotator Rotation)
{
	if (!MagicCircleParticle)
	{
		UE_LOG(LogTemp, Error, TEXT("effect create"));

		MagicCircleParticle = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			MagicCircleEffect2,
			Location,
			Rotation,
			FVector(3.0f),
			false
		);
	}
	else
	{
		MagicCircleParticle->SetWorldLocation(Location);
		MagicCircleParticle->SetWorldRotation(Rotation);
		if (!bIsMagicCircleActive)
		{
			MagicCircleParticle->ActivateSystem(true);
			bIsMagicCircleActive = true;
		}
	}

	bIsMagicCircleActive = true;
	//DrawDebugSphere(GetWorld(), Location, 50.f, 12, FColor::Blue, false, 2.f);
}

void ANoteBookWeapon::StopNoteBookAttack()
{
	//GetWorld()->GetTimerManager().ClearTimer(DamageTickHandle);
}
