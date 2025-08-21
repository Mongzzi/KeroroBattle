// Fill out your copyright notice in the Description page of Project Settings.


#include "NunwawaCharacter.h"
#include "KeroroAnimInstance.h"

ANunwawaCharacter::ANunwawaCharacter(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	EnemyType = EEnemyType::Nunwawa;
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SKMESH(TEXT("/Game/Keroro_Model/nunwawa/Nunwawa.Nunwawa"));
	if (SKMESH.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SKMESH.Object);
	}
}

void ANunwawaCharacter::Attack()
{
	if (!EnemyAnim) return;
	EnemyAnim->PlayEnemyAttackMontage(EnemyType);
}

void ANunwawaCharacter::AttackCheck()
{
}
