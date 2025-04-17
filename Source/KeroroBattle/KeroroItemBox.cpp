// Fill out your copyright notice in the Description page of Project Settings.


#include "KeroroItemBox.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "KeroroWeapon.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
AKeroroItemBox::AKeroroItemBox()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BOX"));

	RootComponent = Trigger;
	Box->SetupAttachment(RootComponent);

	Trigger->SetBoxExtent(FVector(52.0f, 34.5f, 33.5f));
	Trigger->SetCollisionProfileName(TEXT("ItemBox"));

	Box->SetCollisionProfileName(TEXT("NoCollision"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_BOX(TEXT("/Game/SciFiCrates/Crates/Mesh/SM_Small_Crate_V1.SM_Small_Crate_V1"));
	if (SM_BOX.Succeeded())
	{
		Box->SetStaticMesh(SM_BOX.Object);
	}
	WeaponItemClass = AKeroroWeapon::StaticClass();

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>NS(TEXT("/Game/Basic_VFX/Niagara/NS_Basic_6.NS_Basic_6"));
	if (NS.Succeeded())
	{
		NSOpenEffect = NS.Object;
	}

}

// Called when the game starts or when spawned
void AKeroroItemBox::BeginPlay()
{
	Super::BeginPlay();

}

void AKeroroItemBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AKeroroItemBox::OnCharacterBeginOverlap);
}

// Called every frame
void AKeroroItemBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKeroroItemBox::OnCharacterBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("item box ovelpped"));
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NSOpenEffect, GetActorLocation()+FVector(0.0f,0.0f,100.0f), FRotator::ZeroRotator, FVector(3.0f));
	Destroy();
}