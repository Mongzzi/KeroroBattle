// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RifleBullet.generated.h"



UCLASS()
class KEROROBATTLE_API ARifleBullet : public AActor
{
	GENERATED_BODY()

public:
	ARifleBullet();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* Collision;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, Category = "Effect")
	class UNiagaraComponent* BulletEffectComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	class UNiagaraSystem* BulletEffect;

	UPROPERTY(EditAnywhere, Category = Sound)
	TArray<USoundBase*> HitSounds;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	class UNiagaraSystem* HitEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Bullet")
	float BulletSpeed = 6000.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Bullet")
	float LifeSpanTime = 3.0f;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};