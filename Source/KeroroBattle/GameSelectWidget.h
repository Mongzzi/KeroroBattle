// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameSelectWidget.generated.h"

/**
 *
 */
UCLASS()
class KEROROBATTLE_API UGameSelectWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	UPROPERTY()
	class UKeroroGameInstance* KRGI;

	UFUNCTION()
	void OnButtonHoverd();

public:
	UFUNCTION()
	void OnSelectButton1Clicked();

	UFUNCTION()
	void OnSelectButton2Clicked();

	UFUNCTION()
	void OnSelectButton3Clicked();

	UFUNCTION()
	void OnSelectButton4Clicked();

	UFUNCTION()
	void OnSelectButton5Clicked();

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* SelectButton1;

	UPROPERTY(meta = (BindWidget))
	class UButton* SelectButton2;

	UPROPERTY(meta = (BindWidget))
	class UButton* SelectButton3;

	UPROPERTY(meta = (BindWidget))
	class UButton* SelectButton4;

	UPROPERTY(meta = (BindWidget))
	class UButton* SelectButton5;
};
