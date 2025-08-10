// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSelectTrigger.h"
#include "Components/BoxComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "KeroroCharacter.h"
#include "GameSelectWidget.h"


// Sets default values
AGameSelectTrigger::AGameSelectTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AGameSelectTrigger::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AGameSelectTrigger::OnOverlapEnd);


	static ConstructorHelpers::FClassFinder<UGameSelectWidget>WIDGET1(TEXT("/Game/Blueprints/KR_GameSelectWidget.KR_GameSelectWidget_C"));
	if (WIDGET1.Succeeded())
	{
		GameSelectWidgetClass = WIDGET1.Class;
	}
}

// Called when the game starts or when spawned
void AGameSelectTrigger::BeginPlay()
{
	Super::BeginPlay();

}

void AGameSelectTrigger::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (TriggerBox)
	{
		TriggerBox->OnComponentBeginOverlap.RemoveDynamic(this, &AGameSelectTrigger::OnOverlapBegin);
		TriggerBox->OnComponentEndOverlap.RemoveDynamic(this, &AGameSelectTrigger::OnOverlapEnd);
	}
}

// Called every frame
void AGameSelectTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGameSelectTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bHasTriggered) return;
	if (OtherActor && OtherActor->IsA<AKeroroCharacter>())
	{
		bHasTriggered = true;

		if (GameSelectWidgetClass)
		{
			GameSelectWidget = CreateWidget<UGameSelectWidget>(GetWorld(), GameSelectWidgetClass);
			if (GameSelectWidget)
			{
				GameSelectWidget->AddToViewport();

				APlayerController* PC = GetWorld()->GetFirstPlayerController();
				if (PC)
				{
					PC->bShowMouseCursor = true;
					PC->SetInputMode(FInputModeUIOnly());
					PC->SetIgnoreMoveInput(true);
					PC->SetIgnoreLookInput(true);

				}
			}
		}
	}
}

void AGameSelectTrigger::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->IsA<AKeroroCharacter>())
	{
		bHasTriggered = false;
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (PC)
		{
			PC->bShowMouseCursor = false;
			PC->SetInputMode(FInputModeGameOnly());
			PC->SetIgnoreMoveInput(false);
			PC->SetIgnoreLookInput(false);

		}
		if (GameSelectWidget)
		{
			GameSelectWidget->RemoveFromParent();
		}
	}
}

