// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere,Category = Box)
	class UBoxComponent* Trigger;

	UPROPERTY(VisibleAnywhere,Category = Box)
	class UStaticMeshComponent* Box;

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
