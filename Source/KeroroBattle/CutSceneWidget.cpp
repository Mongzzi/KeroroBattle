// Fill out your copyright notice in the Description page of Project Settings.

#include "CutSceneWidget.h"
#include "KeroroPlayerController.h"
#include "KeroroPlayerState.h"
#include "Components/Image.h"
#include "MediaPlayer.h"
#include "MediaTexture.h"
#include "FileMediaSource.h"
#include "MediaSoundComponent.h"

UCutSceneWidget::UCutSceneWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UMediaPlayer> MP(TEXT("/Game/Etc/test2_1.test2_1"));

	if (MP.Succeeded())
	{
		MediaPlayer = MP.Object;
	}
	static ConstructorHelpers::FObjectFinder<UFileMediaSource> MS1(TEXT("/Game/Etc/ulti_kero.ulti_kero"));
	if (MS1.Succeeded())
	{
		KeroroMS = MS1.Object;
	}
	static ConstructorHelpers::FObjectFinder<UFileMediaSource> MS2(TEXT("/Game/Etc/ulti_tama.ulti_tama"));
	if (MS2.Succeeded())
	{
		TamamaMS = MS2.Object;
	}
	static ConstructorHelpers::FObjectFinder<UFileMediaSource> MS3(TEXT("/Game/Etc/ulti_giro.ulti_giro"));
	if (MS3.Succeeded())
	{
		GiroroMS = MS3.Object;
	}
	static ConstructorHelpers::FObjectFinder<UFileMediaSource> MS4(TEXT("/Game/Etc/ulti_doro.ulti_doro"));
	if (MS4.Succeeded())
	{
		DororoMS = MS4.Object;
	}
	static ConstructorHelpers::FObjectFinder<UFileMediaSource> MS5(TEXT("/Game/Etc/ulti_kuru.ulti_kuru"));
	if (MS5.Succeeded())
	{
		KururuMS = MS5.Object;
	}
}

void UCutSceneWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetVisibility(ESlateVisibility::Hidden);

	//if (!MediaSound && MediaPlayer)
	//{
	//	MediaSound = NewObject<UMediaSoundComponent>(this);
	//	MediaSound->SetMediaPlayer(MediaPlayer);
	//	MediaSound->RegisterComponentWithWorld(GetWorld());
	//}
}

void UCutSceneWidget::NativeDestruct()
{
	if (MediaPlayer)
	{
		MediaPlayer->OnEndReached.RemoveDynamic(this, &UCutSceneWidget::OnCutsceneFinished);
	}

	Super::NativeDestruct();
}

void UCutSceneWidget::PlayCutscene()
{
	if (!MediaPlayer) return;

	AKeroroPlayerController* PC = Cast<AKeroroPlayerController>(GetOwningPlayer());
	if (!PC) return;

	AKeroroPlayerState* PS = Cast<AKeroroPlayerState>(PC->PlayerState);
	if (!PS) return;

	EKeroroType CurrentKero = PS->GetCurrentCharacterType();
	UFileMediaSource* MS = nullptr;

	switch (CurrentKero)
	{
	case EKeroroType::Keroro:
		MS = KeroroMS;
		break;
	case EKeroroType::Tamama:
		MS = TamamaMS;
		break;
	case EKeroroType::Giroro:
		MS = GiroroMS;
		break;
	case EKeroroType::Kururu:
		MS = KururuMS;
		break;
	case EKeroroType::Dororo:
		MS = DororoMS;
		break;
	}

	if (!MS) return;

	SetVisibility(ESlateVisibility::Visible);

	if (MediaPlayer->OpenSource(MS))
	{
		MediaPlayer->Play();
		MediaPlayer->OnEndReached.AddDynamic(this, &UCutSceneWidget::OnCutsceneFinished);
	}
}

void UCutSceneWidget::OnCutsceneFinished()
{
	SetVisibility(ESlateVisibility::Hidden);
}
