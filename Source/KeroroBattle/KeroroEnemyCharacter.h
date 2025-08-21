// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KeroroBattle.h"
#include "GameFramework/Character.h"
#include "KeroroEnemyCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnEnemyDie)

UCLASS()
class KEROROBATTLE_API AKeroroEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AKeroroEnemyCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Attack();
	virtual void AttackCheck();

	UFUNCTION()
	void Die();

public:
	float AttackRange;
	float AttackRadius;
	bool bIsDead;

	EEnemyType EnemyType = EEnemyType::Joriri;

	FOnEnemyDie OnEnemyDie;

public:
	class UKeroroAnimInstance* EnemyAnim;

	UPROPERTY(VisibleAnywhere, Category = Stat)
	class UKeroroStatComponent* EnemyStat;

	UPROPERTY(VisibleAnywhere, Category = UI)
	class UWidgetComponent* HPBar;

	UPROPERTY()
	TSubclassOf<class UDamageTextWidget> DamageTextWidgetClass;
};
