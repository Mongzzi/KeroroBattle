// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KeroroBattle.h"
#include "GameFramework/Character.h"
#include "KeroroEnemyCharacter.generated.h"

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

    // 스탯 컴포넌트 (HP, 공격력, 경험치 등 관리)
    UPROPERTY(VisibleAnywhere, Category = Stat)
    class UKeroroStatComponent* EnemyStat;

    UPROPERTY(VisibleAnywhere, Category = UI)
    class UWidgetComponent* HPBar;

    // 몬스터가 공격받을 때 호출되는 함수
    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

    // 몬스터가 죽었을 때 호출되는 함수
    virtual void Die();

    // 사망 처리 여부
    bool bIsDead = false;

private:
    class UKeroroAnimInstance* EnemyAnim;

};
