// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_DetectEnemy.h"
#include "KeroroAIController.h"
#include "KeroroEnemyCharacter.h"
#include "KeroroCharacter.h"
#include "EngineUtils.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_DetectEnemy::UBTService_DetectEnemy()
{
	NodeName = TEXT("KRDetect");
	Interval = 3.0f;
}

// 아래 코드 템플릿 함수로 빼서 관리 하는게 좋을듯 ( 몬스터와 동일하게 동작 )
// 플레이어 ai 클래스임
void UBTService_DetectEnemy::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    APawn* kero = OwnerComp.GetAIOwner()->GetPawn();
    if (kero == nullptr) return;

    AKeroroCharacter* Keroro = Cast<AKeroroCharacter>(OwnerComp.GetAIOwner()->GetCharacter());
    if (Keroro == nullptr) return;

    auto TargetObj = OwnerComp.GetBlackboardComponent()->GetValueAsObject(AKeroroAIController::TargetKey);

    float DetectRadiusSq = 600.0f * 600.0f; 

    AActor* NearestEnemy = nullptr;
    float NearestDistSq = FLT_MAX;

    TArray<FHitResult> HitResults;
    FCollisionQueryParams Params;

    bool bHit = GetWorld()->SweepMultiByChannel(
        HitResults,
        kero->GetActorLocation(),
        kero->GetActorLocation(),
        FQuat::Identity,
        ECC_GameTraceChannel3,
        FCollisionShape::MakeSphere(FMath::Sqrt(DetectRadiusSq)),
        Params
    );

    if (bHit)
    {
        for (const FHitResult& Hit : HitResults)
        {
            AKeroroEnemyCharacter* Enemy = Cast<AKeroroEnemyCharacter>(Hit.GetActor());
            if (Enemy)
            {
                float DistSq = (kero->GetActorLocation() - Enemy->GetActorLocation()).SizeSquared();
                if (DistSq < NearestDistSq)
                {
                    NearestDistSq = DistSq;
                    NearestEnemy = Enemy;
                }
            }
        }
    }

    if (NearestEnemy != TargetObj)
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsObject(AKeroroAIController::TargetKey, NearestEnemy);
        if (NearestEnemy)
        {
            Keroro->StartRun();
        }
        else
        {
            Keroro->StopRun();
        }
    }
}
