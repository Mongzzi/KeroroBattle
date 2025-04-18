// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KeroroWeapon.generated.h"

UCLASS()
class KEROROBATTLE_API AKeroroWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKeroroWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere,Category= Weapon)
	USkeletalMeshComponent* Weapon;

	FVector AttachLocationOffset;	// 상대적 위치( hand_rSocket )
	FRotator AttachRotationOffset; // 상대적 회전 ( hand_rSocket )
	FVector AttachScale;
};
