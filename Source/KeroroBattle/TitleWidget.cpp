// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleWidget.h"
#include "MediaPlayer.h"
#include "MediaTexture.h"
#include "FileMediaSource.h"
#include "MediaSoundComponent.h"

void UTitleWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!MediaSound && MediaPlayer)
	{
		MediaSound = NewObject<UMediaSoundComponent>(this);
		MediaSound->SetMediaPlayer(MediaPlayer);
		MediaSound->RegisterComponentWithWorld(GetWorld());
	}
}

void UTitleWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

UTitleWidget::UTitleWidget(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UMediaPlayer> MP(TEXT("/Game/Etc/test2_1.test2_1"));
	if (MP.Succeeded())
	{
		MediaPlayer = MP.Object;
	}

	static ConstructorHelpers::FObjectFinder<UFileMediaSource> MS1(TEXT("/Game/Etc/TitleVIdeo.TitleVIdeo"));
	if (MS1.Succeeded())
	{
		TitleMediaSource = MS1.Object;
	}
}

void UTitleWidget::PlayTitleMedia()
{
	if (MediaPlayer->OpenSource(TitleMediaSource))
	{
		MediaPlayer->Play();
	}
}
