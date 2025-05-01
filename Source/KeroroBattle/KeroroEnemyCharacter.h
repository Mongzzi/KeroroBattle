// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KeroroBattle.h"
#include "GameFramework/Character.h"
#include "KeroroEnemyCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate)

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
    void Die();
    
    // 사망 처리 여부
    bool bIsDead;

public:
    // 적 몬스터 콤보 공격용
    int32 CurrentComboIndex;
    int32 MaxCombo;        
    float CanComboAttackTime; // 공격 후 다음 콩보공격까지 가능 시간
    float CanComboAttackDist; // 적 캐릭터와 거리
    bool bIsAttacking; // 애니메이션 중인지
    bool bCanNextCombo; // 다음공격 가능한지

    FTimerHandle ComboResetTimerHandle; // CanComboAttackTime을 위한 핸들 

    void Attack();  // 태스크에서 사용
    void ResetCombo();
    
    UFUNCTION()
    void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

    UFUNCTION()
    void EnableNextCombo(); // 노티파이를 통해 다음공격


public:
    class UKeroroAnimInstance* EnemyAnim;
};
