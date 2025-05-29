// Fill out your copyright notice in the Description page of Project Settings.


#include "NoteBookWeapon.h"
#include "KeroroCharacter.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

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
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NS_Magic(TEXT("/Game/MixedVFX/Particles/Mix/NS_ElectricField.NS_ElectricField"));
	if (NS_Magic.Succeeded())
	{
		MagicCircleEffect = NS_Magic.Object;
	}
}

void ANoteBookWeapon::BeginPlay()
{
}

void ANoteBookWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANoteBookWeapon::InitEffect(AKeroroCharacter* kero)
{
	OwnerKero = kero;
}

void ANoteBookWeapon::ActivateMagicCircle()
{
	if (!MagicCircleEffect || !OwnerKero) return;

	APlayerController* PC = Cast<APlayerController>(OwnerKero->GetController());
	if (!PC) return;

	FVector WorldLocation, WorldDirection;
	if (!PC->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
		return;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerKero);

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
			SpawnOrUpdateEffect(HitResult.Location, PC->GetControlRotation());
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
		SpawnOrUpdateEffect(DownHit.Location, Rotation); // 바닥 위에 정렬
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

void ANoteBookWeapon::SpawnOrUpdateEffect(FVector Location, FRotator Rotation)
{
	if (!MagicCircleNiagara)
	{
		UE_LOG(LogTemp, Error, TEXT("effect create"));
		MagicCircleNiagara = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			MagicCircleEffect,
			Location,
			Rotation
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


