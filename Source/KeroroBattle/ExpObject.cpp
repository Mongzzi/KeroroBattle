// Fill out your copyright notice in the Description page of Project Settings.


#include "ExpObject.h"
#include "KeroroCharacter.h"

// Sets default values
AExpObject::AExpObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>EXPOBJECT(TEXT("/Game/Fab/Star_effect_low_poly_137KB/star_effect_low_poly_transparency.star_effect_low_poly_transparency"));
	if (EXPOBJECT.Succeeded())
	{
		MeshComp->SetSkeletalMesh(EXPOBJECT.Object);
		MeshComp->SetRelativeScale3D(FVector(5.0f, 5.0f, 5.0f));  // 전체적으로 절반 크기
	}

	MoveSpeed = 0.0f;

}

// Called when the game starts or when spawned
void AExpObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExpObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!TargetKeroro) return;

	FVector CurrentLoc = GetActorLocation();
	FVector TargetLoc = TargetKeroro->GetActorLocation();
	FVector NewLoc = FMath::VInterpTo(CurrentLoc, TargetLoc, DeltaTime, MoveSpeed);
	SetActorLocation(NewLoc);

	if (FVector::Dist(NewLoc, TargetLoc) < 50.0f)
	{
		// 경험치 증가 처리 여기서 델리게이트 호출하여 경험치 증가 처리하면될듯
		UE_LOG(LogTemp, Error, TEXT("Exp object already come to keroro"));
		Destroy();
	}
}

void AExpObject::SetTargetAndSpeed(AKeroroCharacter* Target, float Speed)
{
	if (IsValid(Target))
	{
		TargetKeroro = Target;
		MoveSpeed = Speed;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("SetTargetAndSpeed is failed , Target is not valid"));
	}
}


