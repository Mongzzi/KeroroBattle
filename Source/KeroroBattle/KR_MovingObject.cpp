// Fill out your copyright notice in the Description page of Project Settings.


#include "KR_MovingObject.h"
#include "KeroroItemBox.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

// Sets default values
AKR_MovingObject::AKR_MovingObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MOVINGOBJ"));
	RootComponent = StaticMeshComponent;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SHIP(TEXT("/Game/Keroro_Model/etc/dropship/KeroDropShip.KeroDropShip"));
	if (SHIP.Succeeded())
	{
		StaticMeshComponent->SetStaticMesh(SHIP.Object);
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> SOUND(TEXT("/Game/Keroro_Sound/kururu/Kuru_Ulti_Sound.Kuru_Ulti_Sound"));
	if (SOUND.Succeeded())
	{
		UltiSound = SOUND.Object;
	}
}

// Called when the game starts or when spawned
void AKR_MovingObject::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(
		DropTimerHandle,
		this,
		&AKR_MovingObject::DropItemBox,
		0.5f, // 2초마다
		true  // 루프 반복
	);

	if (UltiSound)
	{
		PlayingSound = UGameplayStatics::SpawnSoundAtLocation(this, UltiSound, GetActorLocation());
	}
}

// Called every frame
void AKR_MovingObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsMoving) return;

	CurrentDist += MoveSpeed * DeltaTime;
	float t = FMath::Clamp(CurrentDist / TotalDist, 0.0f, 1.0f);

	if (t < 1.0f)
	{
		FVector NextLoc = GetBezierPoint(t);
		FVector Dir = NextLoc - GetActorLocation();
		SetActorRotation(Dir.Rotation());
		SetActorLocation(NextLoc);
	}
	else {
		if (PlayingSound && PlayingSound->IsPlaying())
		{
			PlayingSound->Stop();
		}
		Destroy();
	}
}

void AKR_MovingObject::DropItemBox()
{
	FVector DropLocation = GetActorLocation() - FVector(0.0f, 0.0f, 100.f);
	FRotator DropRotation = FRotator::ZeroRotator;

	auto ItemBox = GetWorld()->SpawnActor<AKeroroItemBox>(AKeroroItemBox::StaticClass(), DropLocation, DropRotation);
	ItemBox->SetPhysics();

}

void AKR_MovingObject::StartMoving(float Speed)
{
	MoveSpeed = Speed;
	bIsMoving = true;
}

void AKR_MovingObject::SetBezierPoints(FVector p0, FVector p1, FVector p2)
{
	P0 = p0;
	P1 = p1;
	P2 = p2;

	TotalDist = GetTotalDistance();
	CurrentDist = 0.0f;
}

float AKR_MovingObject::GetTotalDistance()
{
	float length = 0.0f;
	FVector prev = GetBezierPoint(0.0f);

	// 30개 지점으로 나눠서 계산
	for (int i = 1; i <= 30; ++i)
	{
		float t = i / 30.0f;
		FVector cur = GetBezierPoint(t);
		length += FVector::Dist(prev, cur);
		prev = cur;
	}
	return length;
}

FVector AKR_MovingObject::GetBezierPoint(float t)
{
	FVector a = FMath::Lerp(P0, P1, t);
	FVector b = FMath::Lerp(P1, P2, t);
	return FMath::Lerp(a, b, t);
}

