// Fill out your copyright notice in the Description page of Project Settings.


#include "KeroroGameInstance.h"
#include "Engine/StreamableManager.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"
#include "UISoundManager.h"


UKeroroGameInstance::UKeroroGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable>KRDT(TEXT("/Game/GameData/StatData.StatData"));
	if (KRDT.Succeeded())KRStatTable = KRDT.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable>CDDT(TEXT("/Game/GameData/CardData.CardData"));
	if (CDDT.Succeeded())CardTable = CDDT.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable>FADT(TEXT("/Game/GameData/FaceAssetData.FaceAssetData"));
	if (FADT.Succeeded()) FaceAssetTable = FADT.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable>FSDT(TEXT("/Game/GameData/SoundAssetData.SoundAssetData"));
	if (FSDT.Succeeded())KRSoundAssetTable = FSDT.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable>CDT(TEXT("/Game/GameData/ChatData.ChatData"));
	if (CDT.Succeeded())KRChatTable = CDT.Object;

	static ConstructorHelpers::FObjectFinder<USoundBase> BGM1(TEXT("/Game/Etc/BGMAsset/Robby.Robby"));
	if (BGM1.Succeeded())MapBGMMap.Add(BGM1.Object);

	static ConstructorHelpers::FObjectFinder<USoundBase> BGM2(TEXT("/Game/Etc/BGMAsset/main1.main1"));
	if (BGM2.Succeeded())MapBGMMap.Add(BGM2.Object);

	static ConstructorHelpers::FObjectFinder<USoundBase> BGM3(TEXT("/Game/Etc/BGMAsset/main2.main2"));
	if (BGM3.Succeeded())MapBGMMap.Add(BGM3.Object);

	static ConstructorHelpers::FObjectFinder<USoundBase> BGM4(TEXT("/Game/Etc/BGMAsset/main3.main3"));
	if (BGM4.Succeeded())MapBGMMap.Add(BGM4.Object);

	static ConstructorHelpers::FObjectFinder<USoundBase> BGM5(TEXT("/Game/Etc/BGMAsset/Boss.Boss"));
	if (BGM5.Succeeded())MapBGMMap.Add(BGM5.Object);

	UnlockCharacter(EKeroroType::Keroro);
	UnlockCharacter(EKeroroType::Tamama);
	UnlockCharacter(EKeroroType::Giroro);
	UnlockCharacter(EKeroroType::Dororo);
	UnlockCharacter(EKeroroType::Kururu);

	//UnlockedCharacters.Add(EKeroroType::Tamama, false);
	//UnlockedCharacters.Add(EKeroroType::Giroro, false);
	//UnlockedCharacters.Add(EKeroroType::Dororo, false);
	//UnlockedCharacters.Add(EKeroroType::Kururu, false);

	UnlockedChattings.Add(EKeroroType::Tamama, false);
	UnlockedChattings.Add(EKeroroType::Giroro, false);
	UnlockedChattings.Add(EKeroroType::Dororo, false);
	UnlockedChattings.Add(EKeroroType::Kururu, false);
}


void UKeroroGameInstance::Init()
{
	Super::Init();
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UKeroroGameInstance::OnLevelLoaded);
	UISoundManager = NewObject<UUISoundManager>(this);
}

// 행 이름을 레벨과 같게하여 FindRow함수에 행이름과 같은 레벨을 넣어서 행을 가져옴
FKRStatData* UKeroroGameInstance::GetKRStatData(int Level)
{
	return KRStatTable->FindRow<FKRStatData>(*FString::FromInt(Level), TEXT(""));
}

FCardData* UKeroroGameInstance::GetCardData(int CardID)
{
	return CardTable->FindRow<FCardData>(*FString::FromInt(CardID), TEXT(""));
}

UDataTable* UKeroroGameInstance::GetChatDataTable()
{
	if (KRChatTable) return KRChatTable;
	else return nullptr;
}

FSoftObjectPath UKeroroGameInstance::GetFaceAssetPath(EKeroroType KeroroType, EFaceType FaceType)
{
	if (!FaceAssetTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("FaceAssetTable is null"));
		return FSoftObjectPath();
	}

	FString KeroroTypeName;
	switch (KeroroType)
	{
	case EKeroroType::Keroro: KeroroTypeName = TEXT("Keroro"); break;
	case EKeroroType::Tamama: KeroroTypeName = TEXT("Tamama"); break;
	case EKeroroType::Giroro: KeroroTypeName = TEXT("Giroro"); break;
	case EKeroroType::Kururu: KeroroTypeName = TEXT("Kururu"); break;
	case EKeroroType::Dororo: KeroroTypeName = TEXT("Dororo"); break;
	}

	FString FaceTypeName;
	switch (FaceType)
	{
	case EFaceType::Default: FaceTypeName = TEXT("Default"); break;
	case EFaceType::Sad: FaceTypeName = TEXT("Sad"); break;
	case EFaceType::Happy: FaceTypeName = TEXT("Happy"); break;
	case EFaceType::Fatal: FaceTypeName = TEXT("Fatal"); break;
	case EFaceType::Super: FaceTypeName = TEXT("Super"); break;
	case EFaceType::Anger: FaceTypeName = TEXT("Anger"); break;
	}

	FString RowNameStr = KeroroTypeName + TEXT("Face") + FaceTypeName;
	FName RowName(*RowNameStr);

	FKRFaceAssetData* FoundRow = FaceAssetTable->FindRow<FKRFaceAssetData>(RowName, TEXT(""));

	if (FoundRow)
	{
		return FoundRow->AssetPath;
	}

	UE_LOG(LogTemp, Warning, TEXT("Face asset not found ----- %s"), *RowNameStr);
	return FSoftObjectPath();
}

FSoftObjectPath UKeroroGameInstance::GetVoiceSoundAssetPath(EKeroroType KeroroType, int32 index)
{
	if (!KRSoundAssetTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("KRSoundAssetTable is null"));
		return FSoftObjectPath();
	}

	FString KeroroTypeName;
	switch (KeroroType)
	{
	case EKeroroType::Keroro: KeroroTypeName = TEXT("Keroro"); break;
	case EKeroroType::Tamama: KeroroTypeName = TEXT("Tamama"); break;
	case EKeroroType::Giroro: KeroroTypeName = TEXT("Giroro"); break;
	case EKeroroType::Kururu: KeroroTypeName = TEXT("Kururu"); break;
	case EKeroroType::Dororo: KeroroTypeName = TEXT("Dororo"); break;
	}

	FString RowNameStr = KeroroTypeName + TEXT("AttackSound") + FString::FromInt(index);
	FName RowName(*RowNameStr);

	FKRSoundAssetData* FoundRow = KRSoundAssetTable->FindRow<FKRSoundAssetData>(RowName, TEXT(""));
	if (FoundRow)
	{
		return FoundRow->AssetPath;
	}
	UE_LOG(LogTemp, Warning, TEXT("Sound asset not found ----- %s"), *RowNameStr);
	return FSoftObjectPath();
}

FStreamableManager& UKeroroGameInstance::GetStreamableManager()
{
	return StreamableManager;
}

void UKeroroGameInstance::LoadLevelWithLoadingScreen(FName MapName)
{
	NextMapName = MapName;
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("LoadingLevel")));
}

bool UKeroroGameInstance::IsCharacterUnlocked(EKeroroType Type)
{
	if (UnlockedCharacters.Contains(Type))
	{
		return UnlockedCharacters[Type];
	}
	return false;
}

void UKeroroGameInstance::UnlockCharacter(EKeroroType Type)
{
	if (IsCharacterUnlocked(Type)) return;

	UnlockedCharacters.Add(Type, true);
}

void UKeroroGameInstance::PlayBGM(USoundBase* NewBGM)
{
	if (!NewBGM) return;

	if (CurrentBGMComponent)
	{
		CurrentBGMComponent->Stop();
		CurrentBGMComponent->DestroyComponent();
		CurrentBGMComponent = nullptr;
	}
	CurrentBGMComponent = UGameplayStatics::SpawnSound2D(this, NewBGM);
}

void UKeroroGameInstance::StopBGM()
{
	if (CurrentBGMComponent)
	{
		CurrentBGMComponent->Stop();
		CurrentBGMComponent->DestroyComponent();
		CurrentBGMComponent = nullptr;
	}
}

void UKeroroGameInstance::PlayUISound(EUISoundType Type)
{
	if (UISoundManager)
	{
		UISoundManager->PlayUISound(Type);
	}
}

void UKeroroGameInstance::OnLevelLoaded(UWorld* LoadedWorld)
{
	if (!LoadedWorld) return;

	FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);

	int32 BgmIndex = 0;

	if (CurrentLevelName == TEXT("Robby1Level"))
	{
		BgmIndex = 0;
	}
	else if (CurrentLevelName == TEXT("MainLevel1"))
	{
		BgmIndex = 1;
	}
	else if (CurrentLevelName == TEXT("MainLevel2"))
	{
		BgmIndex = 2;

	}
	else if (CurrentLevelName == TEXT("MainLevel3"))
	{
		if (NextMissionRound == EKeroroType::Dororo)
		{
			BgmIndex = 3;
		}
		else
		{
			BgmIndex = 4; // 보스전
		}
	}
	else if (CurrentLevelName == TEXT("LoadingLevel"))
	{
		StopBGM();
		return;
	}
	PlayBGM(MapBGMMap[BgmIndex]);
}

FKRStatData::FKRStatData()
{
	Level = 1;
}

FCardData::FCardData()
{
	CardID = 1;
}

FKRFaceAssetData::FKRFaceAssetData()
{

}

FKRSoundAssetData::FKRSoundAssetData()
{
}

FKRChatData::FKRChatData()
{
}
