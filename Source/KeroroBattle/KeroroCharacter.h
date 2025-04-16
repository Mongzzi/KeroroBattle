// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KeroroBattle.h"
#include "GameFramework/Character.h"
#include "KeroroCharacter.generated.h"

UCLASS()
class KEROROBATTLE_API AKeroroCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AKeroroCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;
	void Attack();

public:
	// 나이아가라 시스템
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	class UNiagaraSystem* NSAttackEffect;

	// 이펙트 생성
	UFUNCTION()
	void PlaySwordEffect();
	
public:
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	
	void AttackStartComboState();
	void AttackEndComboState();
	void HandleComboInput();
	void StartNewAttack();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsAttacking;

	UPROPERTY(VisibleInstanceOnly,BlueprintReadOnly,Category=Attack,Meta = (AllowPrivateAccess = true))
	bool CanNextCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsComboInputOn;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int32 CurrentCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int32 MaxCombo;

protected:
	void Move(const struct FInputActionValue& Value);
	void Look(const struct FInputActionValue& Value);
	void StartRun();
	void StopRun();

public:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* SpringArm;

public:
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	USkeletalMeshComponent* Weapon;

private:
	// 플레이어 컨트롤러
	UPROPERTY()
	class AKeroroPlayerController* KRPlayerContoller;

	// 애님인스턴스
	UPROPERTY()
	class UKeroroAnimInstance* KRAnim;

protected:
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* Moving;

	UPROPERTY(EditAnyWhere, Category = Input)
	class UInputAction* Jumping;

	UPROPERTY(EditAnyWhere, Category = Input)
	class UInputAction* Looking;

	UPROPERTY(EditAnyWhere, Category = Input)
	class UInputAction* Running;

	UPROPERTY(EditAnyWhere, Category = Input)
	class UInputAction* Attacking;

	UPROPERTY(EditAnywhere, Category = Stat)
	float RunSpeed;

	UPROPERTY(EditAnywhere, Category = Stat)
	float WalkSpeed;

};
