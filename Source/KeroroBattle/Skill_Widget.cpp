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
	UE_LOG(LogTemp, Log, TEXT("HP 회복 아이템 적용"));
	AKeroroCharacter* kero = Cast<AKeroroCharacter>(GetOwningPlayerPawn());
	if (kero)
	{
		UKeroroStatComponent* krstat = kero->KRStat;
		if (krstat)
		{
			krstat->SetHP(krstat->MaxHp);
		}
	}
}

void USkill_Widget::ItemMP()
{
	UE_LOG(LogTemp, Log, TEXT("MP 회복 아이템 적용"));
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
}

void USkill_Widget::ItemShieldCoolZero()
{
	UE_LOG(LogTemp, Log, TEXT("쉴드 쿨타임 초기화"));
	AKeroroCharacter* kero = Cast<AKeroroCharacter>(GetOwningPlayerPawn());
	if (kero)
	{
		kero->SetGuardCooldownReset();
	}
}

void USkill_Widget::ItemUltCoolZero()
{
	UE_LOG(LogTemp, Log, TEXT("궁극기 쿨타임 초기화"));
	AKeroroCharacter* kero = Cast<AKeroroCharacter>(GetOwningPlayerPawn());
	if (kero)
	{
		kero->SetUltiCooldownReset();
	}
}

void USkill_Widget::ItemGold()
{
	UE_LOG(LogTemp, Log, TEXT("골드 획득"));
	AKeroroPlayerController* PC = Cast<AKeroroPlayerController>(GetOwningPlayer());
	if (PC)
	{
		AKeroroPlayerState* PS = Cast<AKeroroPlayerState>(PC->PlayerState);
		if (PS)
		{
			PS->AddGold(300);
		}
	}
}

void USkill_Widget::ItemKillAllEnemies()
{
	UE_LOG(LogTemp, Log, TEXT("모든 적 처치"));

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
}

void USkill_Widget::ItemGroupEnemies()
{
	UE_LOG(LogTemp, Log, TEXT("적 한 곳으로 모으기"));
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
	UE_LOG(LogTemp, Log, TEXT("공격력 증가"));
	
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
}

void USkill_Widget::ItemMoveSpeedUp()
{
	UE_LOG(LogTemp, Log, TEXT("이동속도 증가"));
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
}

void USkill_Widget::ItemDefenseUp()
{
	UE_LOG(LogTemp, Log, TEXT("방어력 증가"));
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
	UE_LOG(LogTemp, Log, TEXT("공업끝"));

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
	UE_LOG(LogTemp, Log, TEXT("이속업끝"));
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
	UE_LOG(LogTemp, Log, TEXT("방어업끝"));

}
