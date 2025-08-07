// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KeroroBattle.h"
#include "Blueprint/UserWidget.h"
#include "TitleWidget.generated.h"

/**
 * 
 */
UCLASS()
class KEROROBATTLE_API UTitleWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	UTitleWidget(const FObjectInitializer& ObjectInitializer);

public:

	void PlayTitleMedia();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Media)
	class UMediaPlayer* MediaPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Media)
	class UMediaSoundComponent* MediaSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Media)
	class UFileMediaSource* TitleMediaSource;

};
