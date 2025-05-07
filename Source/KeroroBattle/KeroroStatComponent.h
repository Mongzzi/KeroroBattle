// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KeroroStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);
DECLARE_MULTICAST_DELEGATE(FOnHPIsChangedDeleGate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KEROROBATTLE_API UKeroroStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UKeroroStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:
	UPROPERTY(EditInstanceOnly, Category = Stat)
	int32 Level;

	UPROPERTY(VisibleAnywhere, Transient, Category = Stat)
	float CurrentHP;

	UPROPERTY(VisibleAnywhere, Transient, Category = Stat)
	float AttackPower;

public:
	void SetLevel(int32 lv);
	void SetDamage(float dm);
	void SetHP(float hp);
	float GetHpRatio();

	FOnHPIsZeroDelegate OnHpIsZero;
	FOnHPIsChangedDeleGate OnHpIsChanged;


private:
	struct FKRStatData* StatData;
};
