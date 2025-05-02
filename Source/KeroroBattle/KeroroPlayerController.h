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
	void Move(const struct FInputActionValue& Value);
	void Look(const struct FInputActionValue& Value);
	void Jump();
	void StartRun();
	void StopRun();
	void Attack();
	void TagCharacter();
	void LoadInputActionAndMappingContext();

public:
	class UNiagaraSystem* NSTagEffect;
	class UNiagaraComponent* NCTagEffect;
	UPROPERTY()
	TMap<EKeroroType, class AKeroroCharacter*> CharacterMap;


	// test
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UKeroroHUDWidget> KRHUDWidgetClass;

	UPROPERTY()
	class UKeroroHUDWidget* KRHUDWidget;

	void UpdateHPWidget();
	void UpdateGoldWidget();
	void UpdateTimeWidget(float RemainTime);
	void UpdateEXPWidget();
	void UpdateKillWidget();
	float GetGameStateRemainingTime();
	void Die();

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
};
