// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleWidget.h"
#include "MediaPlayer.h"
#include "MediaTexture.h"
#include "FileMediaSource.h"
#include "MediaSoundComponent.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "KeroroGameInstance.h"

void UTitleWidget::NativeConstruct()
{
	Super::NativeConstruct();

	KRGI = Cast<UKeroroGameInstance>(GetGameInstance());
	if (!KRGI) return;

	if (!MediaSound && MediaPlayer)
	{
		MediaSound = NewObject<UMediaSoundComponent>(this);
		MediaSound->SetMediaPlayer(MediaPlayer);
		MediaSound->RegisterComponentWithWorld(GetWorld());
	}

	if (StartButton)
	{
		StartButton->OnHovered.AddDynamic(this, &UTitleWidget::OnStartHovered);
		StartButton->OnUnhovered.AddDynamic(this, &UTitleWidget::OnStartUnhovered);
		StartButton->OnPressed.AddDynamic(this, &UTitleWidget::OnStartPressed);
		StartButton->OnReleased.AddDynamic(this, &UTitleWidget::OnStartReleased);
	}

	if (LoadButton)
	{
		LoadButton->OnHovered.AddDynamic(this, &UTitleWidget::OnLoadHovered);
		LoadButton->OnUnhovered.AddDynamic(this, &UTitleWidget::OnLoadUnhovered);
		LoadButton->OnPressed.AddDynamic(this, &UTitleWidget::OnLoadPressed);
		LoadButton->OnReleased.AddDynamic(this, &UTitleWidget::OnLoadReleased);
	}

	if (ExitButton)
	{
		ExitButton->OnHovered.AddDynamic(this, &UTitleWidget::OnExitHovered);
		ExitButton->OnUnhovered.AddDynamic(this, &UTitleWidget::OnExitUnhovered);
		ExitButton->OnPressed.AddDynamic(this, &UTitleWidget::OnExitPressed);
		ExitButton->OnReleased.AddDynamic(this, &UTitleWidget::OnExitReleased);
	}
}

void UTitleWidget::NativeDestruct()
{
	Super::NativeDestruct();
	if (StartButton)
	{
		StartButton->OnHovered.RemoveAll(this);
		StartButton->OnUnhovered.RemoveAll(this);
		StartButton->OnPressed.RemoveAll(this);
		StartButton->OnReleased.RemoveAll(this);
	}

	if (LoadButton)
	{
		LoadButton->OnHovered.RemoveAll(this);
		LoadButton->OnUnhovered.RemoveAll(this);
		LoadButton->OnPressed.RemoveAll(this);
		LoadButton->OnReleased.RemoveAll(this);
	}

	if (ExitButton)
	{
		ExitButton->OnHovered.RemoveAll(this);
		ExitButton->OnUnhovered.RemoveAll(this);
		ExitButton->OnPressed.RemoveAll(this);
		ExitButton->OnReleased.RemoveAll(this);
	}
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

void UTitleWidget::OnStartHovered()
{
	if (StartText)
	{
		StartText->SetColorAndOpacity(FSlateColor(FLinearColor(0.6f, 1.0f, 0.6f)));
		StartText->SetRenderTranslation(FVector2D(0.0f, -5.0f));
		KRGI->PlayUISound(EUISoundType::Hover);
	}
}

void UTitleWidget::OnStartUnhovered()
{
	if (StartText)
	{
		StartText->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f)));
		StartText->SetRenderTranslation(FVector2D(0.0f, 0.0f));
	}
}

void UTitleWidget::OnStartPressed()
{
	if (StartText)
	{
		StartText->SetColorAndOpacity(FSlateColor(FLinearColor(0.3f, 0.5f, 0.3f)));
		StartText->SetRenderTranslation(FVector2D(0.0f, 5.0f));
	}
}

void UTitleWidget::OnStartReleased()
{
	KRGI->ResetGameData();

	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		PC->bShowMouseCursor = false;
		PC->SetInputMode(FInputModeGameOnly());
	}

	KRGI->LoadLevelWithLoadingScreen(TEXT("Robby1Level"));
}

void UTitleWidget::OnLoadHovered()
{
	if (LoadText)
	{
		LoadText->SetColorAndOpacity(FSlateColor(FLinearColor(0.6f, 1.0f, 0.6f)));
		LoadText->SetRenderTranslation(FVector2D(0.0f, -5.0f));
		KRGI->PlayUISound(EUISoundType::Hover);
	}
}

void UTitleWidget::OnLoadUnhovered()
{
	if (LoadText)
	{
		LoadText->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f)));
		LoadText->SetRenderTranslation(FVector2D(0.0f, 0.0f));
	}
}

void UTitleWidget::OnLoadPressed()
{
	if (LoadText)
	{
		LoadText->SetColorAndOpacity(FSlateColor(FLinearColor(0.3f, 0.5f, 0.3f)));
		LoadText->SetRenderTranslation(FVector2D(0.0f, 5.0f));
	}
}

void UTitleWidget::OnLoadReleased()
{
	OnLoadHovered();

	// 로비씬으로 이동
	//1. UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("/Game/EnglishCollege/Maps/SampleScene.SampleScene")), true, TEXT("?game=/Script/KeroroBattle.KeroroGameMode"));

	//2. UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("SampleScene")), true, TEXT("?game=/Script/KeroroBattle.KeroroGameMode"));
	// 1번처럼 경로로 여는경우 게임모드가 잘안열리는 오류있음, 두번째 경우에는 잘됨 마지막인자 option으로 게임모드 설정가능
	// 현재 사용 중인 코드는 에디터에서 게임모드 미리 설정해둔 상태라 잘 작동됨

	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		PC->bShowMouseCursor = false;
		PC->SetInputMode(FInputModeGameOnly());
	}

	KRGI->LoadLevelWithLoadingScreen(TEXT("Robby1Level"));
}

void UTitleWidget::OnExitHovered()
{
	if (ExitText)
	{
		ExitText->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 0.6f, 0.6f)));
		ExitText->SetRenderTranslation(FVector2D(0.0f, -5.0f));
		KRGI->PlayUISound(EUISoundType::Hover);
	}
}

void UTitleWidget::OnExitUnhovered()
{
	if (ExitText)
	{
		ExitText->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f)));
		ExitText->SetRenderTranslation(FVector2D(0.0f, 0.0f));
	}
}

void UTitleWidget::OnExitPressed()
{
	if (ExitText)
	{
		ExitText->SetColorAndOpacity(FSlateColor(FLinearColor(0.5f, 0.3f, 0.3f)));
		ExitText->SetRenderTranslation(FVector2D(0.0f, 5.0f));
	}
}

void UTitleWidget::OnExitReleased()
{
	OnExitHovered();
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
}