// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill_Widget.h"
#include "KeroroGameInstance.h"
#include "KeroroPlayerController.h"
#include "KeroroPlayerState.h"
#include "KeroroCharacter.h"
#include "KeroroEnemyCharacter.h"
#include "KeroroStatComponent.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "DamageTextWidget.h"


USkill_Widget::USkill_Widget(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>NS1(TEXT("/Game/Basic_VFX/Niagara/NS_Basic_9.NS_Basic_9"));
	if (NS1.Succeeded())
	{
		PullEffect = NS1.Object;
	}

	static ConstructorHelpers::FClassFinder<UDamageTextWidget> DAMAGETEXT(TEXT("/Game/Blueprints/KR_DamageWidget.KR_DamageWidget_C"));
	if (DAMAGETEXT.Succeeded())
	{
		DamageTextWidgetClass = DAMAGETEXT.Class;
	}
}

void USkill_Widget::NativeConstruct()
{
	Super::NativeConstruct();

	ItemType = EItemType::None;
}

void USkill_Widget::UseItem()
{
	switch (ItemType)
	{
	case EItemType::None:
		UE_LOG(LogTemp, Error, TEXT("Losing ticket~"))
			break;
	case EItemType::HP:
		ItemHP();
		break;
	case EItemType::MP:
		ItemMP();
		break;
	case EItemType::ShieldCoolZero:
		ItemShieldCoolZero();
		break;
	case EItemType::UltCoolZero:
		ItemUltCoolZero();
		break;
	case EItemType::Gold:
		ItemGold();
		break;
	case EItemType::KillAll:
		ItemKillAllEnemies();
		break;
	case EItemType::GroupEnemies:
		ItemGroupEnemies();
		break;
	case EItemType::LevelUp:
		ItemLevelUp();
		break;
	case EItemType::AttackUp:
		ItemAttackUp();
		break;
	case EItemType::MoveSpeedUp:
		ItemMoveSpeedUp();
		break;
	case EItemType::DefenseUp:
		ItemDefenseUp();
		break;
	}
	ItemType = EItemType::None;
	SetSkillImageNon();
}

void USkill_Widget::SetItem(EItemType type)
{
	SetItemType(type);
	SetItemImage(type);
}

void USkill_Widget::SetItemImage(EItemType type)
{
	UTexture2D* Image = nullptr;

	switch (type)
	{
	case EItemType::HP:
		Image = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Texture/itemhp.itemhp"));
		break;
	case EItemType::MP:
		Image = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Texture/itemmp.itemmp"));
		break;
	case EItemType::ShieldCoolZero:
		Image = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Texture/itemguard.itemguard"));
		break;
	case EItemType::UltCoolZero:
		Image = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Texture/itemulticool.itemulticool"));
		break;
	case EItemType::Gold:
		Image = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Texture/itemgold.itemgold"));
		break;
	case EItemType::KillAll:
		Image = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Texture/itemalldeath.itemalldeath"));
		break;
	case EItemType::GroupEnemies:
		Image = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Texture/itemgroup.itemgroup"));
		break;
	case EItemType::LevelUp:
		Image = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Texture/itemlevelup.itemlevelup"));
		break;
	case EItemType::AttackUp:
		Image = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Texture/itemattackup.itemattackup"));
		break;
	case EItemType::MoveSpeedUp:
		Image = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Texture/itemspeedup.itemspeedup"));
		break;
	case EItemType::DefenseUp:
		Image = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Texture/itemguardup.itemguardup"));
		break;
	}
	if (Image) SkillImage->SetBrushFromTexture(Image);

	FLinearColor TransparentColor = SkillImage->ColorAndOpacity;
	TransparentColor.A = 1.0f;
	SkillImage->SetColorAndOpacity(TransparentColor);

}

void USkill_Widget::SetSkillImageNon()
{
	if (SkillImage)
	{
		SkillImage->SetBrushFromTexture(nullptr);

		FLinearColor TransparentColor = SkillImage->ColorAndOpacity;
		TransparentColor.A = 0.0f;
		SkillImage->SetColorAndOpacity(TransparentColor);
	}
}

void USkill_Widget::SetSkillImage(int32 Image_num)
{
	AKeroroPlayerController* PC = Cast<AKeroroPlayerController>(GetOwningPlayer());
	FCardData* CardData = PC->GetGameInstance<UKeroroGameInstance>()->GetCardData(Image_num);

	if (CardData)
	{
		UTexture2D* Image = LoadObject<UTexture2D>(nullptr, *CardData->ImagePath.ToString());
		if (SkillImage && Image)
		{
			SkillImage->SetBrushFromTexture(Image);
		}

	}
	else {
		UE_LOG(LogTemp, Error, TEXT("CardData is nullptr"));
	}
}

void USkill_Widget::SetBorderImage(EWidgetType Type)
{
	if (!SkillBorder) {
		UE_LOG(LogTemp, Error, TEXT("SkillBorder is nullptr"));
		return;
	}

	if (Type == EWidgetType::SHIELD_IMAGE)
	{
		FSoftObjectPath ShieldTexturePath(TEXT("/Game/Blueprints/ShieldBorder.ShieldBorder"));
		UTexture2D* Image = LoadObject<UTexture2D>(nullptr, *ShieldTexturePath.ToString());

		if (Image)
		{
			SkillBorder->SetBrushFromTexture(Image);
		}
	}

	else if (Type == EWidgetType::ITEM_IMAGE)
	{
		FSoftObjectPath ShieldTexturePath(TEXT("/Game/Texture/ItemSlot.ItemSlot"));
		UTexture2D* Image = LoadObject<UTexture2D>(nullptr, *ShieldTexturePath.ToString());

		if (Image)
		{
			SkillBorder->SetBrushFromTexture(Image);
		}
	}
}

void USkill_Widget::SetItemSlotNum(int32 num)
{
	if (SlotName)
	{
		FString Str = FString::FromInt(num) + TEXT("번 아이템");
		SlotName->SetText(FText::FromString(Str));
	}
}

void USkill_Widget::UpdateCoolTimeText(float Time)
{
	if (CoolTimeText)
	{
		if (Time < 0.0f)
		{
			CoolTimeText->SetText(FText::GetEmpty());
		}
		else
		{
			CoolTimeText->SetText(FText::AsNumber(static_cast<int32>(Time)));
		}
	}
}

void USkill_Widget::UpdateCoolTimeProgressBar(float Percent)
{
	if (CoolTImeProgressBar)
	{
		CoolTImeProgressBar->SetPercent(Percent);
	}
}


void USkill_Widget::ItemHP()
{
	AKeroroCharacter* kero = Cast<AKeroroCharacter>(GetOwningPlayerPawn());
	if (kero)
	{
		UKeroroStatComponent* krstat = kero->KRStat;
		if (krstat)
		{
			krstat->SetHP(krstat->MaxHp);
		}
	}
	SetTextFromString(FString("HP RECHARGE"),FVector(1.0f,0.0f,0.0f),FVector2D(1.0f));
}

void USkill_Widget::ItemMP()
{
	// 플레이어 MP += 회복량
	AKeroroCharacter* kero = Cast<AKeroroCharacter>(GetOwningPlayerPawn());
	if (kero)
	{
		UKeroroStatComponent* krstat = kero->KRStat;
		if (krstat)
		{
			krstat->SetMP(krstat->MaxMp);
		}
	}
	SetTextFromString(FString("MP RECHARGE"), FVector(0.0f, 0.0f, 1.0f), FVector2D(1.0f));
}

void USkill_Widget::ItemShieldCoolZero()
{
	AKeroroCharacter* kero = Cast<AKeroroCharacter>(GetOwningPlayerPawn());
	if (kero)
	{
		kero->SetGuardCooldownReset();
	}
	SetTextFromString(FString("RESET SHIELD"), FVector(0.0f, 0.0f, 1.0f), FVector2D(1.0f));
}

void USkill_Widget::ItemUltCoolZero()
{
	AKeroroCharacter* kero = Cast<AKeroroCharacter>(GetOwningPlayerPawn());
	if (kero)
	{
		kero->SetUltiCooldownReset();
	}
	SetTextFromString(FString("RESET SKILL"), FVector(0.0f, 1.0f, 0.0f), FVector2D(1.0f));
}

void USkill_Widget::ItemGold()
{
	AKeroroPlayerController* PC = Cast<AKeroroPlayerController>(GetOwningPlayer());
	if (PC)
	{
		AKeroroPlayerState* PS = Cast<AKeroroPlayerState>(PC->PlayerState);
		if (PS)
		{
			PS->AddGold(300);
		}
	}
	SetTextFromString(FString("300 GOLD GET"), FVector(1.0f, 1.0f, 0.0f), FVector2D(1.0f));
}

void USkill_Widget::ItemKillAllEnemies()
{
	AKeroroCharacter* kero = Cast<AKeroroCharacter>(GetOwningPlayerPawn());
	if (!kero) return;

	const FVector Kero_Loc = kero->GetActorLocation();
	float KillRadius = 2000.f;

	TArray<FOverlapResult> HitResults;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(kero);

	bool bHit = GetWorld()->OverlapMultiByChannel(
		HitResults,
		Kero_Loc,
		FQuat::Identity,
		ECC_GameTraceChannel3,
		FCollisionShape::MakeSphere(KillRadius),
		Params
	);

	if (bHit)
	{
		for (auto& Hit : HitResults)
		{
			AKeroroEnemyCharacter* Enemy = Cast<AKeroroEnemyCharacter>(Hit.GetActor());
			if (Enemy)
			{
				Enemy->Die();
			}
		}
	}
	SetTextFromString(FString("ALL KILL"), FVector(0.0f, 0.0f, 0.0f), FVector2D(5.0f));

}

void USkill_Widget::ItemGroupEnemies()
{
	UE_LOG(LogTemp, Log, TEXT("적 한 곳으로 모으기"));
	AKeroroCharacter* Kero = Cast<AKeroroCharacter>(GetOwningPlayerPawn());
	if (!Kero) return;

	FVector KeroLoc = Kero->GetActorLocation();
	FRotator ControlRotation = Kero->GetControlRotation();
	FRotator YawRotation(0.0f, ControlRotation.Yaw, 0.0f);
	FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	float Radius = 3000.0f;
	PullCenter = KeroLoc + ForwardVector * 800.0f + FVector(0, 0, 100);

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AKeroroEnemyCharacter::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		auto* Enemy = Cast<AKeroroEnemyCharacter>(Actor);
		if (!Enemy) continue;

		float Dist = FVector::Dist(Enemy->GetActorLocation(), PullCenter);
		if (Dist > Radius) continue;

		FPullingEnemy Pulling_Info;
		Pulling_Info.Enemy = Enemy;
		Pulling_Info.InitialLocation = Enemy->GetActorLocation();
		Pulling_Info.bHasArrived = false;

		PulledEnemies.Add(Pulling_Info);
	}


	PullEffectComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), PullEffect, PullCenter- FVector(0.0f,0.0f,500.0f), Kero->GetActorRotation(), FVector(10.0f), true);

	GetWorld()->GetTimerManager().SetTimer(PullEffectTimerHandle, this, &USkill_Widget::EndPullEffect, PullDuration*3);

	StartPull();
}

void USkill_Widget::ItemLevelUp()
{
	UE_LOG(LogTemp, Log, TEXT("레벨업 처리"));
	AKeroroPlayerController* PC = Cast<AKeroroPlayerController>(GetOwningPlayer());
	if (PC)
	{
		AKeroroPlayerState* PS = Cast<AKeroroPlayerState>(PC->PlayerState);
		if (PS)
		{
			PS->SetLevel(PS->CurrentLevel + 1);	// clamp 처리 추가해야할듯 나중에 최대레벨 생기면
		}
	}
}

void USkill_Widget::ItemAttackUp()
{
	AKeroroCharacter* kero = Cast<AKeroroCharacter>(GetOwningPlayerPawn());
	if (!kero) return;

	UKeroroStatComponent* krstat = kero->KRStat;
	if (!krstat)return;

	AKeroroPlayerController* PC = Cast<AKeroroPlayerController>(GetOwningPlayer());
	if (!PC) return;

	AKeroroPlayerState* PS = Cast<AKeroroPlayerState>(PC->PlayerState);
	if (!PS) return;

	PS->AttackPower_Enhanced += EnhanceValue_AttackUp;

	FTimerHandle  AttackUpEndHandle;
	GetWorld()->GetTimerManager().SetTimer(AttackUpEndHandle, this, &USkill_Widget::EndAttackUp, 5.0f, false);
	krstat->UpdateStatCardEnhanced(PS);
	
	SetTextFromString(FString("ATTACK POWER UP"), FVector(1.0f, 1.0f, 1.0f), FVector2D(1.0f));

}

void USkill_Widget::ItemMoveSpeedUp()
{
	AKeroroCharacter* kero = Cast<AKeroroCharacter>(GetOwningPlayerPawn());
	if (!kero) return;

	UKeroroStatComponent* krstat = kero->KRStat;
	if (!krstat)return;

	AKeroroPlayerController* PC = Cast<AKeroroPlayerController>(GetOwningPlayer());
	if (!PC) return;

	AKeroroPlayerState* PS = Cast<AKeroroPlayerState>(PC->PlayerState);
	if (!PS) return;

	PS->MaxMoveSpeed_Enhanced += EnhanceValue_SpeedUp;

	FTimerHandle  SpeedUpEndHandle;
	GetWorld()->GetTimerManager().SetTimer(SpeedUpEndHandle, this, &USkill_Widget::EndSpeedUp, 7.0f, false);
	krstat->UpdateStatCardEnhanced(PS);

	SetTextFromString(FString("SPEED POWER UP"), FVector(1.0f, 1.0f, 1.0f), FVector2D(1.0f));

}

void USkill_Widget::ItemDefenseUp()
{
	AKeroroCharacter* kero = Cast<AKeroroCharacter>(GetOwningPlayerPawn());
	if (!kero) return;

	UKeroroStatComponent* krstat = kero->KRStat;
	if (!krstat)return;

	AKeroroPlayerController* PC = Cast<AKeroroPlayerController>(GetOwningPlayer());
	if (!PC) return;

	AKeroroPlayerState* PS = Cast<AKeroroPlayerState>(PC->PlayerState);
	if (!PS) return;

	PS->DefenseRate_Enhanced += EnhanceValue_DefenceUp;

	FTimerHandle  DefenceUpEndHandle;
	GetWorld()->GetTimerManager().SetTimer(DefenceUpEndHandle, this, &USkill_Widget::EndDefenceUp, 5.0f, false);
	krstat->UpdateStatCardEnhanced(PS);

	SetTextFromString(FString("DEFENCE POWER UP"), FVector(1.0f, 1.0f, 1.0f), FVector2D(1.0f));

}

void USkill_Widget::EndAttackUp()
{
	AKeroroCharacter* kero = Cast<AKeroroCharacter>(GetOwningPlayerPawn());
	if (!kero) return;
	UKeroroStatComponent* krstat = kero->KRStat;
	if (!krstat)return;
	AKeroroPlayerController* PC = Cast<AKeroroPlayerController>(GetOwningPlayer());
	if (!PC) return;
	AKeroroPlayerState* PS = Cast<AKeroroPlayerState>(PC->PlayerState);
	if (!PS) return;
	PS->AttackPower_Enhanced -= EnhanceValue_AttackUp;
	krstat->UpdateStatCardEnhanced(PS);

	SetTextFromString(FString("END ATTACK POWER"), FVector(1.0f, 1.0f, 1.0f), FVector2D(1.0f));
}

void USkill_Widget::EndSpeedUp()
{
	AKeroroCharacter* kero = Cast<AKeroroCharacter>(GetOwningPlayerPawn());
	if (!kero) return;
	UKeroroStatComponent* krstat = kero->KRStat;
	if (!krstat)return;
	AKeroroPlayerController* PC = Cast<AKeroroPlayerController>(GetOwningPlayer());
	if (!PC) return;
	AKeroroPlayerState* PS = Cast<AKeroroPlayerState>(PC->PlayerState);
	if (!PS) return;
	PS->MaxMoveSpeed_Enhanced -= EnhanceValue_SpeedUp;
	krstat->UpdateStatCardEnhanced(PS);

	SetTextFromString(FString("END SPEED POWER"), FVector(1.0f, 1.0f, 1.0f), FVector2D(1.0f));
}

void USkill_Widget::EndDefenceUp()
{
	AKeroroCharacter* kero = Cast<AKeroroCharacter>(GetOwningPlayerPawn());
	if (!kero) return;
	UKeroroStatComponent* krstat = kero->KRStat;
	if (!krstat)return;
	AKeroroPlayerController* PC = Cast<AKeroroPlayerController>(GetOwningPlayer());
	if (!PC) return;
	AKeroroPlayerState* PS = Cast<AKeroroPlayerState>(PC->PlayerState);
	if (!PS) return;
	PS->DefenseRate_Enhanced -= EnhanceValue_DefenceUp;
	krstat->UpdateStatCardEnhanced(PS);

	SetTextFromString(FString("END DEFENCE POWER"), FVector(1.0f, 1.0f, 1.0f), FVector2D(1.0f));
}

void USkill_Widget::SetTextFromString(FString Str, FVector Color, FVector2D Scale)
{
	AKeroroCharacter* kero = Cast<AKeroroCharacter>(GetOwningPlayerPawn());
	if (!kero) return;

	AKeroroPlayerController* PC = Cast<AKeroroPlayerController>(GetOwningPlayer());
	if (!PC) return;

	UDamageTextWidget* DamageWidget = CreateWidget<UDamageTextWidget>(PC, DamageTextWidgetClass);
	DamageWidget->AddToViewport();
	FVector Dir = kero->GetActorForwardVector();
	DamageWidget->SetTargetLocation(kero->GetActorLocation() + FVector(Dir.X * -50, 0.0f, -50.0f));
	DamageWidget->SetTextFromString(Str,Color, Scale);
}

void USkill_Widget::StartPull()
{
	PullElapsed = 0.0f;
	GetWorld()->GetTimerManager().SetTimer(
		PullTimerHandle,
		this,
		&USkill_Widget::UpdatePull,
		0.01f,
		true
	);

}

void USkill_Widget::UpdatePull()
{
	PullElapsed += 0.01f;
	float Alpha = FMath::Clamp(PullElapsed / PullDuration, 0.0f, 1.0f);

	for (FPullingEnemy& Enemy_Info : PulledEnemies)
	{
		if (!Enemy_Info.Enemy || Enemy_Info.bHasArrived) continue;

		FVector NewLoc = FMath::Lerp(Enemy_Info.InitialLocation, PullCenter, Alpha);
		Enemy_Info.Enemy->SetActorLocation(NewLoc);

		if (FVector::Dist(NewLoc, PullCenter) < 50.0f)
		{
			Enemy_Info.bHasArrived = true;
			Enemy_Info.Enemy->SetActorLocation(PullCenter);
		}
	}

	if (PullElapsed >= PullDuration)
	{
		EndPull();
	}
}

void USkill_Widget::EndPull()
{
	GetWorld()->GetTimerManager().ClearTimer(PullTimerHandle);

	for (FPullingEnemy& Enemy_Info : PulledEnemies)
	{
		if (!Enemy_Info.Enemy) continue;

		if (Enemy_Info.bHasArrived)
		{
			FVector Offset = FVector(
				FMath::RandRange(-300.0f, 300.0f),
				FMath::RandRange(-300.0f, 300.0f),
				FMath::RandRange(0.0f, 200.0f)
			);
			Enemy_Info.Enemy->SetActorLocation(PullCenter + Offset);
		}
	}
	PulledEnemies.Empty();
}

void USkill_Widget::EndPullEffect()
{
	if (PullEffectComponent)
	{
		PullEffectComponent->Deactivate(); 
		PullEffectComponent->DestroyComponent();
		PullEffectComponent = nullptr;
	}
	GetWorld()->GetTimerManager().ClearTimer(PullEffectTimerHandle);
}
