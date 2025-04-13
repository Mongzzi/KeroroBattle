// Fill out your copyright notice in the Description page of Project Settings.


#include "KeroroAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"

UKeroroAnimInstance::UKeroroAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	IsInAir = false;
	bIsMovingBackward = false;
	bIsRunning = false;
}

void UKeroroAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn))
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
	}
	auto Character = Cast<ACharacter>(Pawn);
	if (Character)
	{
		IsInAir = Character->GetMovementComponent()->IsFalling();
	}

}
