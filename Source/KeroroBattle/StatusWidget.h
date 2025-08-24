// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatusWidget.generated.h"

/**
 * 
 */
UCLASS()
class KEROROBATTLE_API UStatusWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	UPROPERTY()
	class UKeroroGameInstance* KRGI;
	
public:
	
	void SetCardInfoText();
	
	UFUNCTION()
	void OnSelectXButtonClicked();

	UFUNCTION()
	void OnSelectNextButtonClicked();

	UFUNCTION()
	void OnSelectPrevButtonClicked();

	UFUNCTION()
	void OnButtonHoverd();

private:
	int32 NumPage;

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* XButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* NextButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* PrevButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PageText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBox1;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBox2;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBox3;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBox4;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBox5;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBox6;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBox7;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBox8;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBox9;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBox10;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBox11;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBox12;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBox13;
};
