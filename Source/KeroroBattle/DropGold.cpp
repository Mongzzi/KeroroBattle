// Fill out your copyright notice in the Description page of Project Settings.


#include "DropGold.h"
#include "KeroroCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
ADropGold::ADropGold()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Gold = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BOX"));

	RootComponent = Trigger;

	Gold->SetupAttachment(RootComponent);
	Gold->SetCollisionProfileName(TEXT("NoCollision"));

	TriggerBoxExtent = FVector(50.0f, 50.0f, 20.0f);
	Trigger->SetBoxExtent(TriggerBoxExtent);

	// 트리거가 루트컴포넌트여서 트리거의 위치를 바꿔야 메시도 같이 바뀜
	Trigger->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));		
	Trigger->SetCollisionProfileName(TEXT("ItemBox"));
	
	// 트리거박스 크기 디버깅용
	//Trigger->SetHiddenInGame(false);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>GOLDOBJ(TEXT("/Game/Fab/Coin/coin.coin"));
	if (GOLDOBJ.Succeeded())
	{
		Gold->SetStaticMesh(GOLDOBJ.Object);
		Gold->SetRelativeScale3D(FVector(10.0f, 10.0f, 10.0f));
		Gold->SetCollisionProfileName(TEXT("ItemBox"));
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>NS(TEXT("/Game/Basic_VFX/Niagara/NS_Basic_6.NS_Basic_6"));
	if (NS.Succeeded())
	{
		NSEffect = NS.Object;
	}

}

// 에디터에서 트리거박스 크기 직접 수정하기 위함
void ADropGold::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (Trigger)
	{
		Trigger->SetBoxExtent(TriggerBoxExtent);
	}
}

// Called when the game starts or when spawned
void ADropGold::BeginPlay()
{
	Super::BeginPlay();
}

void ADropGold::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ADropGold::OnCharacterBeginOverlap);

}

// Called every frame
void ADropGold::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADropGold::OnCharacterBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<AKeroroCharacter>(OtherActor))
	{
		UE_LOG(LogTemp, Error, TEXT("gold is overlap"));

		NCEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NSEffect, GetActorLocation() + FVector(0.0f, 0.0f, 100.0f), FRotator::ZeroRotator, FVector(3.0f));

		if (NCEffect)
		{
			NCEffect->Activate();

			FTimerHandle EffectTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(EffectTimerHandle, [this]()
				{
					if (NCEffect)
					{
						NCEffect->Deactivate();
					}
				}, 1.0f, false);
		}
		Destroy();
	}
}

