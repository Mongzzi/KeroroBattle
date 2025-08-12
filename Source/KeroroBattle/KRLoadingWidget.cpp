// Fill out your copyright notice in the Description page of Project Settings.


#include "KRLoadingWidget.h"
#include "MediaPlayer.h"
#include "FileMediaSource.h"
#include "Components/Image.h"


void UKRLoadingWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UKRLoadingWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

UKRLoadingWidget::UKRLoadingWidget(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UMediaPlayer> MP(TEXT("/Game/Etc/LoadingAsset/LoadingMediaPlayer.LoadingMediaPlayer"));
	if (MP.Succeeded())
	{
		MediaPlayer = MP.Object;
	}

	static ConstructorHelpers::FObjectFinder<UFileMediaSource> MS1(TEXT("/Game/Etc/LoadingAsset/LoadingAsset.LoadingAsset"));
	if (MS1.Succeeded())
	{
		MediaSource = MS1.Object;
	}
}

void UKRLoadingWidget::StartLoading()
{
	if (MediaPlayer)
	{
		if (MediaPlayer->OpenSource(MediaSource))
		{
			MediaPlayer->SetLooping(true);
			MediaPlayer->Play();
		}
	}
	if (LoadingAnim)
	{
		PlayAnimation(LoadingAnim,0.0f,0);
	}
}
