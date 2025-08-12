// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KRMissionEndWidget.generated.h"

/**
 *
 */
UCLASS()
class KEROROBATTLE_API UKRMissionEndWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;


public:
	void PlayMissionEndAnim();
	
	UFUNCTION()
	void OnClickedRobbyButton();
	
	UFUNCTION()
	void OnClickedExitButton();

protected:	
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* MissionAnim;

	UPROPERTY(meta= (BindWidget))
	class UImage* ResultImage;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* ResultText;

	UPROPERTY(meta =(BindWidget))
	class UButton* RobbyButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ExitButton;
};
