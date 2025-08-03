// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KeroroBattle.h"
#include "GameFramework/Actor.h"
#include "KeroroItemBox.generated.h"


UCLASS()
class KEROROBATTLE_API AKeroroItemBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKeroroItemBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void SetPhysics();

public:
	UPROPERTY(VisibleAnywhere,Category = Box)
	class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditInstanceOnly, Category=Box)
	TSubclassOf<class AKeroroWeapon> WeaponItemClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Effect)
	class UNiagaraSystem* NSOpenEffect;

	UPROPERTY()
	class UNiagaraComponent* NCOpenEffect;

private:
	UFUNCTION()
	void OnCharacterBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
