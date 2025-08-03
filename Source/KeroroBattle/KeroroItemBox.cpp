// Fill out your copyright notice in the Description page of Project Settings.


#include "KeroroItemBox.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "KeroroCharacter.h"
#include "KeroroWeapon.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "KeroroPlayerController.h"
#include "KeroroHUDWidget.h"
#include "Skill_Widget.h"

// Sets default values
AKeroroItemBox::AKeroroItemBox()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BOX"));
	StaticMeshComponent->SetCollisionProfileName(TEXT("ItemBox"));
	RootComponent = StaticMeshComponent;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_BOX(TEXT("/Game/SciFiCrates/Crates/Mesh/SM_Small_Crate_V1.SM_Small_Crate_V1"));
	if (SM_BOX.Succeeded())
	{
		StaticMeshComponent->SetStaticMesh(SM_BOX.Object);
	}
	WeaponItemClass = AKeroroWeapon::StaticClass();

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>NS(TEXT("/Game/Basic_VFX/Niagara/NS_Basic_6.NS_Basic_6"));
	if (NS.Succeeded())
	{
		NSOpenEffect = NS.Object;
	}

}

// Called when the game starts or when spawned
void AKeroroItemBox::BeginPlay()
{
	Super::BeginPlay();
}

void AKeroroItemBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	StaticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AKeroroItemBox::OnCharacterBeginOverlap);
}

void AKeroroItemBox::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (StaticMeshComponent)
	{
		StaticMeshComponent->OnComponentBeginOverlap.RemoveDynamic(this, &AKeroroItemBox::OnCharacterBeginOverlap);

	}
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void AKeroroItemBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKeroroItemBox::SetPhysics()
{
	StaticMeshComponent->SetSimulatePhysics(true);
	float XImpulse = FMath::FRandRange(-400.0f, 400.0f);
	float YImpulse = FMath::FRandRange(-400.0f, 400.0f);
	float ZImpulse = FMath::FRandRange(-400.0f, -100.0f);
	StaticMeshComponent->AddImpulse(FVector(XImpulse, YImpulse, ZImpulse), NAME_None, true);
}

void AKeroroItemBox::OnCharacterBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AKeroroCharacter* kero = Cast<AKeroroCharacter>(OtherActor);
	if (!kero) return;

	// æ∆¿Ã≈€ »πµÊ - ¡æ∑˘ ∫∞ √≥∏Æ
	AKeroroPlayerController* PC = Cast<AKeroroPlayerController>(kero->GetController());
	if (PC && PC->KRHUDWidget)
	{
		TArray<USkill_Widget*> ItemSlots = { PC->KRHUDWidget->ItemWidget1,PC->KRHUDWidget->ItemWidget2,PC->KRHUDWidget->ItemWidget3 };

		// ∫Û ΩΩ∑‘¿ª √£æ∆º≠ √≥∏Æ
		for (auto& Slot : ItemSlots)
		{
			if (Slot && Slot->GetItemType() == EItemType::None)
			{
				int32 MinIdx = static_cast<int32>(EItemType::None) + 1;
				int32 MaxIdx = static_cast<int32>(EItemType::MAX) - 1;
				int32 RandIdx = FMath::RandRange(MinIdx, MaxIdx);
				EItemType RandomType = static_cast<EItemType>(RandIdx);

				Slot->SetItem(RandomType);
				break;
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("item box overlapped"));

	NCOpenEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NSOpenEffect, GetActorLocation() + FVector(0.0f, 0.0f, 100.0f), FRotator::ZeroRotator, FVector(3.0f));

	if (NCOpenEffect)
	{
		NCOpenEffect->Activate();

		FTimerHandle EffectTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(EffectTimerHandle, [this]()
			{
				if (NCOpenEffect)
				{
					NCOpenEffect->Deactivate();
				}
			}, 1.0f, false);
	}
	Destroy();
}