// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KeroroBattle.h"
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
	virtual void NativeDestruct() override;

public:
	void PlayCutscene();

	UFUNCTION()
	void OnCutsceneFinished();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget)
	class UMediaPlayer* MediaPlayer;

	// 영상 사운드 ( 따로 사운드 에셋 플레이 할 예정 일단 냅둠 )
	UPROPERTY()
	class UMediaSoundComponent* MediaSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MediaSource)
	class UFileMediaSource* KeroroMS;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MediaSource)
	class UFileMediaSource* TamamaMS;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MediaSource)
	class UFileMediaSource* GiroroMS;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MediaSource)
	class UFileMediaSource* DororoMS;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MediaSource)
	class UFileMediaSource* KururuMS;

public:
	UPROPERTY(meta = (BindWidget))
	class UImage* CutSceneImage;
};
