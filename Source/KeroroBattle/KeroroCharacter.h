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

public:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* SpringArm;

private:
	UPROPERTY()
	class AKeroroPlayerController* KRPlayerContoller;

protected:
	UPROPERTY(EditAnywhere, Category =Input)
	class UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* Moving;

	UPROPERTY(EditAnyWhere, Category = Input)
	class UInputAction* Jumping;

	UPROPERTY(EditAnyWhere, Category = Input)
	class UInputAction* Looking;

	void Move(const struct FInputActionValue& Value);
	void Look(const struct FInputActionValue& Value);

};
