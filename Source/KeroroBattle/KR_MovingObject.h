// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KR_MovingObject.generated.h"

UCLASS()
class KEROROBATTLE_API AKR_MovingObject : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AKR_MovingObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void DropItemBox();

public:
	void StartMoving(float Speed);
	void SetBezierPoints(FVector p0, FVector p1, FVector p2);
	float GetTotalDistance();
	FVector GetBezierPoint(float t);

private:
	FVector P0, P1, P2;
	
	float TotalDist = 0.0f;
	float CurrentDist = 0.0f;
	float MoveSpeed = 300.f;
	bool bIsMoving = true;

	FTimerHandle DropTimerHandle;

private:
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY()
	USoundWave* UltiSound;

	UPROPERTY()
	UAudioComponent* PlayingSound;
};
