// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KREnemySpawner.generated.h"

UCLASS()
class KEROROBATTLE_API AKREnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKREnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void SpawnEnemy();

protected:
	UPROPERTY()
	TSubclassOf<class AKeroroEnemyCharacter> EnemyClass;

	float SpawnInterval = 1.5f;

	FTimerHandle SpawnTimerHandle;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* SpawnBox;	// 에디터에서 위치 보기위함

	UPROPERTY(EditAnywhere, Category = Spawner)
	int32 MaxEnemyCount = 100;
};
