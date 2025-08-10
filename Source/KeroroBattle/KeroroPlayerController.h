// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KeroroBattle.h"
#include "GameFramework/PlayerController.h"
#include "KeroroPlayerController.generated.h"


USTRUCT(BlueprintType)
struct FSpawnKero
{
	GENERATED_BODY()

public:
	FSpawnKero() : KeroroCharacter(nullptr), bIsSpawnedOnce(false) {};
	UPROPERTY()
	class AKeroroCharacter* KeroroCharacter;

	UPROPERTY()
	bool bIsSpawnedOnce;
};


UCLASS()
class KEROROBATTLE_API AKeroroPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void SetupInputComponent() override;

public:
	AKeroroPlayerController();

	class AKeroroPlayerState* KRPlayerState;

	virtual void OnPossess(APawn* PawnToPossess) override;

	virtual void PostInitializeComponents() override;

private:
	void LoadInputActionAndMappingContext();
	void Move(const struct FInputActionValue& Value);
	void Look(const struct FInputActionValue& Value);
	void Roll(const struct FInputActionValue& Value);
	void Jump();
	void StartRun();
	void StopRun();
	void Attack();
	void TagCharacter(EKeroroType TargetType);
	void Guard();
	void UltimateSkill();
	void OnPlayerLevelUpdated();
	void OnMagicCircleActivated();

	bool IsMagicCircleActivated;
	bool IsRobbyMap = true;

public:
	class UNiagaraSystem* NSTagEffect;
	class UNiagaraComponent* NCTagEffect;

	UPROPERTY()
	TMap<EKeroroType, FSpawnKero> CharacterMap;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UKeroroHUDWidget> KRHUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UKeroroEffectWidget> KRParryWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UCameraShakeBase> KRParryCameraShakeClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UCameraShakeBase> KRUltiSkillCameraShakeClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UCutSceneWidget> KRCutSceneClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UStatusWidget> KRStatusWidgetClass;

	UPROPERTY()
	class UKeroroHUDWidget* KRHUDWidget;

	UPROPERTY()
	class UKeroroEffectWidget* KRParryWidget;

	UPROPERTY()
	class UCutSceneWidget* KRUltimateCutSceneWidget;

	UPROPERTY()
	class UStatusWidget* KRStatusWidget;

	void UpdateStatCardEnhanced();
	void UpdateStatWidget();
	void UpdateHPWidget();
	void UpdateMPWidget();
	void UpdateGoldWidget();
	void UpdateLevelWidget();
	void UpdateTimeWidget(float RemainTime);
	void UpdateEXPWidget();
	void UpdateKillWidget();

	void Die();
	void DieAIKero(EKeroroType type);

	void PlayUltimateCutScene();
	void PlayParryWidgetEffect();
	void PlayParryCameraShake();
	void PlayUltiSkillCameraShake();
	void ShowStatusWidget();
	void UseItemSlotZ();
	void UseItemSlotX();
	void UseItemSlotC();

	void SetUIMode();
	void SetGameMode();
	float GetGameStateRemainingTime();

	void TagKeroro();
	void TagTamama();
	void TagGiroro();
	void TagDororo();
	void TagKururu();

protected:

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* Moving;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* Looking;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* Jumping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* RollingForward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* Running;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* Attacking;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category= Input)
	class UInputAction* MouseRight;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input)
	class UInputAction* Guarding;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* SkillAction;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category= Input)
	class UInputAction* ShowStatus;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* Num1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* Num2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* Num3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* Num4;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* Num5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* ItemZ;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* ItemX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* ItemC;
};
