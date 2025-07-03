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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;
	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void PossessedBy(AController* NewController) override;

public:
	// 이펙트 생성
	UFUNCTION()
	void PlayEffect();

	UFUNCTION()
	void Die();

public:
	// 애님 인스턴스 새로 얻기 및 델리게이트 재바인딩
	void BindCharacterEvents();
	void UnbindCharacterEvents();
	void LoadAssetandSetting(EKeroroType type);

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

private:
	FTimerHandle ParryTimerHandle;
	FTimerHandle GuardTimerHandle;
	FTimerHandle GuardCooldownTimer;

public:
	void StartRun();
	void StopRun();
	void Attack();
	void AttackStartComboState();
	void AttackEndComboState();
	void AttackCheck();
	void AttackCheck_Sword();
	void AttackCheck_Rifle();
	void AttackCheck_Keroball();
	void AttackCheck_Fist();
	void AttackCheck_NoteBook();
	void ParryAttack();
	void ResetGuardCooldown();
	void StartGuard();
	void EndParry();
	void EndGuard();
	void SpawnShieldEffect();
	void DestroyShieldEffect();
	void SpawnToHand();
	void ChangeFaceTexture(EKeroroType KeroroType, EFaceType FaceType);
	void ChangeFaceTexture(EFaceType FaceType);
	void HandleComboInput();
	void StartNewAttack();
	void PlayVoiceSound();
	void PlayHitSound(int32 Combo = 0);
	void PlayHitEffect(FVector HitLocation, FRotator HitRotator, FVector Scale = FVector(1.0f));
	void PlayWeaponSound();
	bool GetIsAttacking() { return IsAttacking; };


public:
	EKeroroType GetKeroroCharacterType() { return CurrentKeroroType; }
	void SetKeroroCharacterType(EKeroroType type) { CurrentKeroroType = type; }


private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsAttacking;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool CanNextCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsComboInputOn;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool CanGuarding = true;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsGuarding = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsParrying = false;


	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int32 CurrentCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int32 MaxCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float AttackRange;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float AttackRadius;

public:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* SpringArm;

	void SetWeapon();
	EWeaponType WeaponType;

public:
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	class AKeroroWeapon* Weapon;

	UPROPERTY(VisibleAnywhere, Category = UI)
	class UWidgetComponent* HPBar;

	UPROPERTY(VisibleAnywhere, Category = Stat)
	class UKeroroStatComponent* KRStat;

	// 애님인스턴스
	UPROPERTY()
	class UKeroroAnimInstance* KRAnim;

	// 플레이어 스테이트
	UPROPERTY()
	class AKeroroPlayerState* KRPlayerState;

private:
	// 플레이어 컨트롤러
	UPROPERTY()
	class AKeroroPlayerController* KRPlayerContoller;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KeroroType)
	EKeroroType CurrentKeroroType = EKeroroType::Dororo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat)
	float RunSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat)
	float WalkSpeed;

public:
	UPROPERTY()
	int32 NumVoices = 4;

	UPROPERTY(EditAnywhere, Category = Sound)
	TArray<USoundBase*> VoiceSounds;

	UPROPERTY(EditAnywhere, Category = Face)
	TArray<UTexture2D*> FaceTextures;

	UPROPERTY()
	UMaterialInstanceDynamic* FaceMaterialInstance;

public:
	// 나이아가라 시스템
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	class UNiagaraSystem* NSRifleEffect;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Shield)
	class UNiagaraSystem* NSShieldEffect;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Shield)
	class UNiagaraSystem* NSGuardEffect;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Shield)
	class UNiagaraSystem* NSParryEffect;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Shield)

	class UNiagaraComponent* NCShieldEffect;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Shield)

	class UNiagaraComponent* NCGuardEffect;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Shield)

	class UNiagaraComponent* NCParryEffect;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Shield)
	class UParticleSystem* PSParryEffect;

	UPROPERTY()
	UParticleSystemComponent* PCParryEffect;

};
