// Fill out your copyright notice in the Description page of Project Settings.


#include "NunwawaCharacter.h"
#include "KeroroAnimInstance.h"
#include "SnowBall.h"

ANunwawaCharacter::ANunwawaCharacter(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	EnemyType = EEnemyType::Nunwawa;
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SKMESH(TEXT("/Game/Keroro_Model/nunwawa/Nunwawa.Nunwawa"));
	if (SKMESH.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SKMESH.Object);
	}
	AttackRange = 1500.0f;
}

void ANunwawaCharacter::Attack()
{
	if (!EnemyAnim) return;
	EnemyAnim->PlayEnemyAttackMontage(EnemyType);
}

void ANunwawaCharacter::AttackCheck()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnParams.Instigator = this;

	FName SocketName = TEXT("SnowWeapon");
	USkeletalMeshComponent* MeshComp = GetMesh();

	if (MeshComp && MeshComp->DoesSocketExist(SocketName))
	{
		FVector SpawnLocation = MeshComp->GetSocketLocation(SocketName);

		GetWorld()->SpawnActor<ASnowBall>(ASnowBall::StaticClass(), SpawnLocation, GetActorRotation(), SpawnParams);
	}
}
