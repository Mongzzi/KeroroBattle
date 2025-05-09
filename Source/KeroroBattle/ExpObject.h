// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExpObject.generated.h"

UCLASS()
class KEROROBATTLE_API AExpObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExpObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetTargetAndSpeed(class AKeroroCharacter* Target, float Speed);




public:
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* MeshComp;

	class AKeroroCharacter* TargetKeroro;
	float MoveSpeed;

};
