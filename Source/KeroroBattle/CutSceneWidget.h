// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CutSceneWidget.generated.h"

/**
 * 
 */
UCLASS()
class KEROROBATTLE_API UCutSceneWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UCutSceneWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	void PlayCutscene();

	UFUNCTION()
	void OnCutsceneFinished();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget)
	class UMediaPlayer* MediaPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget)
	class UFileMediaSource* MediaSource;

	// 영상 사운드
	UPROPERTY()
	class UMediaSoundComponent* MediaSound;

	UPROPERTY(meta = (BindWidget))
	class UImage* CutSceneImage;
};
