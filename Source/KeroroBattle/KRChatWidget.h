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
	void ChangeText(FString str);

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ChatText;

	UPROPERTY(meta = (BindWidget))
	class UButton* NextChatButton;
};
