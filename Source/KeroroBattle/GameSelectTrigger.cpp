// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSelectTrigger.h"
#include "Components/BoxComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "KeroroCharacter.h"


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

		UE_LOG(LogTemp, Error, TEXT("Trigger~~~~"));
		//if (GameSelectWidgetClass)
		//{
		//	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), GameSelectWidgetClass);
		//	if (Widget)
		//	{
		//		Widget->AddToViewport();

		//		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		//		if (PC)
		//		{
		//			PC->SetPause(true);
		//			PC->bShowMouseCursor = true;

		//			FInputModeUIOnly InputMode;
		//			InputMode.SetWidgetToFocus(Widget->TakeWidget());
		//			PC->SetInputMode(InputMode);
		//		}
		//	}
		//}
	}
}

void AGameSelectTrigger::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->IsA<AKeroroCharacter>())
	{
		bHasTriggered = false;
	}
}

