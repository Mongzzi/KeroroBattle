// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KeroroBattle.h"
#include "GameFramework/PlayerController.h"
#include "KeroroPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class KEROROBATTLE_API AKeroroPlayerController : public APlayerController
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
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
	void Jump();
	void StartRun();
	void StopRun();
	void Attack();
	void TagCharacter();
	void Guard();
	void UltimateSkill();
	void OnPlayerLevelUpdated();
	void OnMagicCircleActivated();

	bool IsMagicCircleActivated;

public:
	class UNiagaraSystem* NSTagEffect;
	class UNiagaraComponent* NCTagEffect;
	UPROPERTY()
	TMap<EKeroroType, class AKeroroCharacter*> CharacterMap;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UKeroroHUDWidget> KRHUDWidgetClass;

	UPROPERTY()
	class UKeroroHUDWidget* KRHUDWidget;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UKeroroEffectWidget> KRParryWidgetClass;

	UPROPERTY()
	class UKeroroEffectWidget* KRParryWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCameraShakeBase> KRParryCameraShakeClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UCutSceneWidget> KRCutSceneClass;

	UPROPERTY()
	class UCutSceneWidget* KRUltimateCutSceneWidget;


	void UpdateStatCardEnhanced();
	void UpdateStatWidget();
	void UpdateHPWidget();
	void UpdateGoldWidget();
	void UpdateLevelWidget();
	void UpdateTimeWidget(float RemainTime);
	void UpdateEXPWidget();
	void UpdateKillWidget();

	void Die();
	
	void PlayUltimateCutScene();
	void PlayParryWidgetEffect();
	void PlayParryCameraShake();
	float GetGameStateRemainingTime();
	
	void SetUIMode();
	void SetGameMode();

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
	class UInputAction* Running;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* Attacking;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* Tag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category= Input)
	class UInputAction* MouseRight;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input)
	class UInputAction* Guarding;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* SkillAction;
};
