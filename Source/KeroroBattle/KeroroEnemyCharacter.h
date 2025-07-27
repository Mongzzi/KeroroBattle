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
	AKeroroEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
    virtual void PostInitializeComponents() override;

    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
    void Attack();
    void AttackCheck();

    UFUNCTION()
    void Die();
    
public:
    // 적 몬스터 콤보 공격용
    float AttackRange; // 적 캐릭터와 거리
    float AttackRadius;
    bool bIsDead;
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
