// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KRChatWidget.generated.h"

/**
 *
 */
UCLASS()
class KEROROBATTLE_API UKRChatWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	int32 ChatID;
	int32 ImageID;
	int32 NextChatID1;
	int32 NextChatID2;

public:
	UFUNCTION()
	void UpdateChatInfo(int32 _ChatID,int32 _ImageID, FString _ChatText, TArray<FString> _SelectChatTexts, TArray<FString> _NextChatIDs);

	UFUNCTION()
	void OnSelectButton1();

	UFUNCTION()
	void OnSelectButton2();

	UFUNCTION()
	void OnSelectNextButton();

	void PlayFadeInAnim();

private:
	class AKeroroPlayerController* PC;

private:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* FadeIn;

	UPROPERTY(meta = (BindWidget))
	class UImage* CharacterImage;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ChatText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SelectText1;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SelectText2;

	UPROPERTY(meta = (BindWidget))
	class UButton* NextChatButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* SelectButton1;

	UPROPERTY(meta = (BindWidget))
	class UButton* SelectButton2;
};
