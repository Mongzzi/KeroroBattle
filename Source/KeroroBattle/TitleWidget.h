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

public:
	UPROPERTY()
	class UKeroroGameInstance* KRGI;

protected:
	UPROPERTY(meta = (BindWidget))
	class UButton* StartButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* LoadButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* StartText;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* LoadText;

	UPROPERTY(meta = (BindWidget))
	class UButton* ExitButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ExitText;

	UFUNCTION()
	void OnStartHovered();

	UFUNCTION()
	void OnStartUnhovered();

	UFUNCTION()
	void OnStartPressed();

	UFUNCTION()
	void OnStartReleased();

	UFUNCTION()
	void OnLoadHovered();

	UFUNCTION()
	void OnLoadUnhovered();

	UFUNCTION()
	void OnLoadPressed();

	UFUNCTION()
	void OnLoadReleased();

	UFUNCTION()
	void OnExitHovered();

	UFUNCTION()
	void OnExitUnhovered();

	UFUNCTION()
	void OnExitPressed();

	UFUNCTION()
	void OnExitReleased();

};
