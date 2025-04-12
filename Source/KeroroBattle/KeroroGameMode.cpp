// Fill out your copyright notice in the Description page of Project Settings.


#include "KeroroGameMode.h"
#include "KeroroCharacter.h"
#include "KeroroPlayerController.h"

AKeroroGameMode::AKeroroGameMode()
{
	DefaultPawnClass = AKeroroCharacter::StaticClass();
	PlayerControllerClass = AKeroroPlayerController::StaticClass();

}
