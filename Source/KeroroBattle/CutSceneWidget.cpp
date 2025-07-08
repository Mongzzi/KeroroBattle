// Fill out your copyright notice in the Description page of Project Settings.


#include "CutSceneWidget.h"
#include "Components/Image.h"
#include "MediaPlayer.h"
#include "MediaTexture.h"
#include "FileMediaSource.h"
#include "MediaSoundComponent.h"

UCutSceneWidget::UCutSceneWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UMediaPlayer> MP(TEXT("/Game/Etc/test2_1.test2_1"));
	static ConstructorHelpers::FObjectFinder<UFileMediaSource> MS(TEXT("/Game/Etc/test1.test1"));

	if (MP.Succeeded() && MS.Succeeded())
	{
		MediaPlayer = MP.Object;
		MediaSource = MS.Object;
	}
}

void UCutSceneWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetVisibility(ESlateVisibility::Hidden);

	if (!MediaSound && MediaPlayer)
	{
		MediaSound = NewObject<UMediaSoundComponent>(this);
		MediaSound->SetMediaPlayer(MediaPlayer);
		MediaSound->RegisterComponentWithWorld(GetWorld());
	}
}

void UCutSceneWidget::PlayCutscene()
{
	if (!MediaPlayer || !MediaSource) return;

	SetVisibility(ESlateVisibility::Visible);

	if (MediaPlayer->OpenSource(MediaSource))
	{
		MediaPlayer->Play();
		MediaPlayer->OnEndReached.AddDynamic(this, &UCutSceneWidget::OnCutsceneFinished);
	}
}

void UCutSceneWidget::OnCutsceneFinished()
{
	SetVisibility(ESlateVisibility::Hidden);
}
