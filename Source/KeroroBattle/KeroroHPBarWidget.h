// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KeroroHPBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class KEROROBATTLE_API UKeroroHPBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindKRStat(class UKeroroStatComponent* NewKRStat);

protected:
	virtual void NativeConstruct() override;
	void UpdateHPWidget();

private:
	TWeakObjectPtr<class UKeroroStatComponent> CurrentKRStat;

	// 블루프린트에 있는 변수명 그대로 가져와야함
	UPROPERTY(meta= (BindWidget))
	class UProgressBar* Hp_Bar;

};
