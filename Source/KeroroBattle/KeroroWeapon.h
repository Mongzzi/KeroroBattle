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

	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	virtual void PlayEffect();
	virtual void PlaySound();

	FName GetSocketName() { return SocketName; };
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	USkeletalMeshComponent* SKMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	UStaticMeshComponent* StaticMeshComponent;

protected:
	FName SocketName;
};
