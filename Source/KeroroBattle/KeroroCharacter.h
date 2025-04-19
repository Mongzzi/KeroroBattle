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

public:
	// 나이아가라 시스템
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	class UNiagaraSystem* NSAttackEffect;

	// 이펙트 생성
	UFUNCTION()
	void PlaySwordEffect();
	
public:
	// 애님 인스턴스 새로 얻기 및 델리게이트 재바인딩
	void BindAnimInstanceEvents();

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	
public:
	void StartRun();
	void StopRun();
	void Attack();
	void AttackStartComboState();
	void AttackEndComboState();
	void HandleComboInput();
	void StartNewAttack();
	void LoadAssetandSetting(EKeroroType type);

public:
	EKeroroType GetKeroroCharacterType() { return CurrentKeroroType; }
	void SetKeroroCharacterType(EKeroroType type) { CurrentKeroroType = type; }

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

	// 스트림핸들 저장하기위해선 꼭 TSharedPtr 타입으로 해줘야함 
	TSharedPtr<struct FStreamableHandle> AssetStreamingHandle;


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EKeroroType CurrentKeroroType = EKeroroType::Keroro;

	UPROPERTY(EditAnywhere, Category = Stat)
	float RunSpeed;

	UPROPERTY(EditAnywhere, Category = Stat)
	float WalkSpeed;

};
