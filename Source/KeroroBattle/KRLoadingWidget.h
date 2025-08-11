// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KRLoadingWidget.generated.h"


/**
 *
 */
UCLASS()
class KEROROBATTLE_API UKRLoadingWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	UKRLoadingWidget(const FObjectInitializer& ObjectInitializer);
	
public:
	
	void StartLoading();
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Media)
	class UMediaPlayer* MediaPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Media)
	class UFileMediaSource* MediaSource;

	UPROPERTY(meta = (BindWidget))
	class UImage* LoadingImage;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* LoadingAnim;
};
