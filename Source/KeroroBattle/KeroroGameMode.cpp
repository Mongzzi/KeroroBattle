// Fill out your copyright notice in the Description page of Project Settings.


#include "KeroroGameMode.h"
#include "KeroroCharacter.h"
#include "KeroroPlayerController.h"
#include "KeroroPlayerState.h"

AKeroroGameMode::AKeroroGameMode()
{
	DefaultPawnClass = AKeroroCharacter::StaticClass();
	PlayerControllerClass = AKeroroPlayerController::StaticClass();
	PlayerStateClass = AKeroroPlayerState::StaticClass();
}
