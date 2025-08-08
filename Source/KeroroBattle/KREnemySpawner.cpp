// Fill out your copyright notice in the Description page of Project Settings.


#include "KREnemySpawner.h"
#include "KeroroEnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"

// Sets default values
AKREnemySpawner::AKREnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnBox"));
	RootComponent = SpawnBox;
	SpawnBox->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));

	SpawnBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SpawnBox->SetHiddenInGame(true);

	EnemyClass = AKeroroEnemyCharacter::StaticClass();
}

// Called when the game starts or when spawned
void AKREnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	if (EnemyClass)
	{
		GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle,this,&AKREnemySpawner::SpawnEnemy,SpawnInterval,true);
	}
}

// Called every frame
void AKREnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKREnemySpawner::SpawnEnemy()
{
	TArray<AActor*> FoundEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AKeroroEnemyCharacter::StaticClass(), FoundEnemies);

	if (FoundEnemies.Num() >= MaxEnemyCount)
	{
		// 너무 많으면 스폰하지 않음
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRotation = GetActorRotation();

	GetWorld()->SpawnActor<AKeroroEnemyCharacter>(EnemyClass, SpawnLocation, SpawnRotation, SpawnParams);
}

