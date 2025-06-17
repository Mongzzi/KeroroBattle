// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DropGold.generated.h"

UCLASS()
class KEROROBATTLE_API ADropGold : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADropGold();
	virtual void OnConstruction(const FTransform& Transform);
	void SetGoldMount(int32 goldmount) { DropGoldMount = goldmount; };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger")
	FVector TriggerBoxExtent;

	UPROPERTY(VisibleAnywhere, Category = Gold)
	class UBoxComponent* Trigger;

	UPROPERTY(VisibleAnywhere,Category= Gold)
	class UStaticMeshComponent* Gold;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Effect)
	class UNiagaraSystem* NSEffect;
	int32 DropGoldMount;
	
	
private:
	UFUNCTION()
	void OnCharacterBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
