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
	int32 NextChatID1;
	int32 NextChatID2;

public:
	UFUNCTION()
	void UpdateChatInfo(int32 chatid, FString chattext, TArray<FString> SelectChatTexts, TArray<FString> nextchatids);

	UFUNCTION()
	void OnSelectButton1();

	UFUNCTION()
	void OnSelectButton2();

	UFUNCTION()
	void OnSelectNextButton();

private:
	TWeakObjectPtr<class UChatManager> ChatManager;
	TWeakObjectPtr<class AKeroroPlayerController> PC;

private:
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
