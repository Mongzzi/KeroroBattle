// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "KeroroPlayerController.h"
#include "KeroroPlayerState.h"

void UStatusWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (XButton)
	{
		XButton->OnClicked.AddDynamic(this, &UStatusWidget::OnSelectXButtonClicked);
	}

	if (NextButton)
	{
		NextButton->OnClicked.AddDynamic(this, &UStatusWidget::OnSelectNextButtonClicked);
	}

	if (PrevButton)
	{
		PrevButton->OnClicked.AddDynamic(this, &UStatusWidget::OnSelectPrevButtonClicked);
	}

	NumPage = 0;

	SetCardInfoText();
}

void UStatusWidget::NativeDestruct()
{
	Super::NativeDestruct();
	if (XButton)
	{
		XButton->OnClicked.RemoveDynamic(this, &UStatusWidget::OnSelectXButtonClicked);
	}

	if (NextButton)
	{
		NextButton->OnClicked.RemoveDynamic(this, &UStatusWidget::OnSelectNextButtonClicked);
	}
}

void UStatusWidget::SetCardInfoText()
{

	AKeroroPlayerController* PC = Cast<AKeroroPlayerController>(GetOwningPlayer());
	if (PC)
	{
		AKeroroPlayerState* PS = Cast<AKeroroPlayerState>(PC->PlayerState);
		if (PS)
		{
			if (NumPage == 0)
			{
				if (TextBox1) TextBox1->SetText(FText::FromString(FString::Printf(TEXT("최대 체력 +%.0f"), PS->MaxHP_Enhanced)));
				if (TextBox2) TextBox2->SetText(FText::FromString(FString::Printf(TEXT("최대 마나 증가 +%.0f"), PS->MaxMP_Enhanced)));
				if (TextBox3) TextBox3->SetText(FText::FromString(FString::Printf(TEXT("이동속도 증가 +%.0f"), PS->MaxMoveSpeed_Enhanced)));
				if (TextBox4) TextBox4->SetText(FText::FromString(FString::Printf(TEXT("치명타 확률 증가 +%.0f%%"), PS->CritChanceRate_Enhanced * 100)));
				if (TextBox5) TextBox5->SetText(FText::FromString(FString::Printf(TEXT("치명타 피해율 증가 +%.0f%%"), PS->CritDamageRate_Enhanced * 100)));
				if (TextBox6) TextBox6->SetText(FText::FromString(FString::Printf(TEXT("무적 시간 증가 +%.3f초"), PS->InvincibilityTime_Enhanced)));
				if (TextBox7) TextBox7->SetText(FText::FromString(FString::Printf(TEXT("스킬 쿨타임 감소 -%.0f%%"), PS->SkillCooldownRate_Enhanced * 100)));
				if (TextBox8) TextBox8->SetText(FText::FromString(FString::Printf(TEXT("회피율 증가 +%.0f%%"), PS->EvasionRate_Enhanced * 100)));
				if (TextBox9) TextBox9->SetText(FText::FromString(FString::Printf(TEXT("방어율 증가 +%.0f%%"), PS->DefenseRate_Enhanced * 100)));
				if (TextBox10) TextBox10->SetText(FText::FromString(FString::Printf(TEXT("가드 쿨타임 감소 -%.0f%%"), PS->GuardCoolTime_Enhanced * 100)));
				if (TextBox11) TextBox11->SetText(FText::FromString(FString::Printf(TEXT("가드 판정 타임 증가  +%.0f%%"), PS->GuardTime_Enhanced * 100)));
				if (TextBox12) TextBox12->SetText(FText::FromString(FString::Printf(TEXT("패링 판정 타임 +%.0f%%"), PS->ParryTime_Enhanced * 100)));

			}
			else if (NumPage == 1)
			{
				if (TextBox1) TextBox1->SetText(FText::FromString(FString::Printf(TEXT("경험치 획득율 증가 +%.0f%%"), (PS->ExpGainRate - 1.0f) * 100)));
				if (TextBox2) TextBox2->SetText(FText::FromString(FString::Printf(TEXT("골드 획득율 증가 +%.0f%%"), (PS->GoldGainRate - 1.0f) * 100)));
				if (TextBox3) TextBox3->SetText(FText::FromString(FString::Printf(TEXT("체력 재생율 증가 +%.3f%%"), PS->HealPowerRate_Enhanced * 100)));
				if (TextBox4) TextBox4->SetText(FText::FromString(FString::Printf(TEXT("적 처치 시 체력 회복 증가 +%.3f%%"), PS->HealPowerOnKill_Enhanced * 100)));
				if (TextBox5) TextBox5->SetText(FText::FromString(FString::Printf(TEXT("마나 재생율 증가 +%.3f%%"), PS->MpHealPowerRate_Enhanced * 100)));
				if (TextBox6) TextBox6->SetText(FText::FromString(FString::Printf(TEXT("적 처치 시 마나회복 증가 +%.3f%%"), PS->MpHealPowerOnKill_Enhanced * 100)));
				if (TextBox7) TextBox7->SetText(FText::FromString(FString::Printf(TEXT("발사체 크기 증가 +%.0f%%"), PS->ProjectileScale_Enhanced * 100)));
				if (TextBox8) TextBox8->SetText(FText::FromString(FString::Printf(TEXT("발사체 수 증가 +%d개"), PS->ProjectileCount_Enhanced)));
				if (TextBox9) TextBox9->SetText(FText::FromString(""));
				if (TextBox10) TextBox10->SetText(FText::FromString(""));
				if (TextBox11) TextBox11->SetText(FText::FromString(""));
				if (TextBox12) TextBox12->SetText(FText::FromString(""));
			}

		}
	}
}

void UStatusWidget::OnSelectXButtonClicked()
{
	AKeroroPlayerController* PC = Cast<AKeroroPlayerController>(GetOwningPlayer());
	if (PC && PC->KRStatusWidget)
	{
		PC->KRStatusWidget->RemoveFromParent();
		PC->SetGameMode();
	}
}

void UStatusWidget::OnSelectNextButtonClicked()
{
	if (NumPage == 0)
	{
		NumPage = 1;
		if (PageText)
		{
			PageText->SetText(FText::FromString("Prev Page"));
		}
		SetCardInfoText();
	}
}

void UStatusWidget::OnSelectPrevButtonClicked()
{
	if (NumPage == 1)
	{
		NumPage = 0;
		if (PageText)
		{
			PageText->SetText(FText::FromString("Next Page"));
		}
		SetCardInfoText();
	}
}
