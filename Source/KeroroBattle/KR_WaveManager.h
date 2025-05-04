// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KR_WaveManager.generated.h"

UCLASS()
class KEROROBATTLE_API AKR_WaveManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKR_WaveManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
    UPROPERTY(EditAnywhere, Category = Wave)
    TSubclassOf<class AKeroroEnemyCharacter> EnemyClass;

    UPROPERTY(EditAnywhere, Category = Wave)
    float SpawnRadius;

    UPROPERTY(EditAnywhere, Category = Wave)
    int32 BaseEnemyCount;
    int32 MaxWave;
    int32 CurrentWave;
    int32 EnemiesToSpawn;
    int32 EnemiesAlive;

    void StartWave();
    void SpawnEnemies();

    UFUNCTION()
    void OnEnemyDead(AActor* DestroyedActor);

};
