// Fill out your copyright notice in the Description page of Project Settings.


#include "KR_WaveManager.h"
#include "KeroroEnemyCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AKR_WaveManager::AKR_WaveManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	EnemyClass = AKeroroEnemyCharacter::StaticClass();

	SpawnRadius = 400.0f;

	BaseEnemyCount = 1;
	CurrentWave = 0;
	MaxWave = 5;
	EnemiesToSpawn = 0;
	EnemiesAlive = 0;
}

// Called when the game starts or when spawned
void AKR_WaveManager::BeginPlay()
{
	Super::BeginPlay();
	StartWave();
}

// Called every frame
void AKR_WaveManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKR_WaveManager::StartWave()
{
	if (CurrentWave >= MaxWave)
	{
		UE_LOG(LogTemp, Warning, TEXT("All waves completed!"));
		return;
	}

	CurrentWave++;
	EnemiesToSpawn = BaseEnemyCount + CurrentWave * 1;
	EnemiesAlive = EnemiesToSpawn;

	UE_LOG(LogTemp, Log, TEXT("=== Starting Wave %d ==="), CurrentWave);
	SpawnEnemies();
}

void AKR_WaveManager::SpawnEnemies()
{
	if (!EnemyClass) return;

	AActor* Player = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!Player) return;

	FVector PlayerLocation = Player->GetActorLocation()+FVector(0.0f,0.0f,300.0f);

	for (int32 i = 0; i < EnemiesToSpawn; ++i)
	{
		float Angle = FMath::RandRange(0.f, 360.f);

		// 플레이어 기준 원형 x(cos) , y(sin) 값
		FVector Offset = FVector(FMath::Cos(FMath::DegreesToRadians(Angle)), FMath::Sin(FMath::DegreesToRadians(Angle)), 0.0f) * SpawnRadius;

		FVector SpawnLocation = PlayerLocation + Offset;
		FRotator SpawnRotation = (PlayerLocation - SpawnLocation).Rotation();

		AKeroroEnemyCharacter* SpawnedEnemy = GetWorld()->SpawnActor<AKeroroEnemyCharacter>(EnemyClass, SpawnLocation, SpawnRotation);
		if (SpawnedEnemy)
		{
			SpawnedEnemy->OnDestroyed.AddDynamic(this, &AKR_WaveManager::OnEnemyDead);
		}
		else if (SpawnedEnemy == nullptr)
		{
			EnemiesAlive--;
		}
	}
}

void AKR_WaveManager::OnEnemyDead(AActor* DestroyedActor)
{
	EnemiesAlive--;

	UE_LOG(LogTemp, Log, TEXT("Enemy killed Remaining: %d"), EnemiesAlive);

	if (EnemiesAlive <= 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Wave %d cleared!"), CurrentWave);
		StartWave();
	}
}

