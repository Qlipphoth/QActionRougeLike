// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/QBTService_CheckAttackRange.h"
#include "AI/QAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UQBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
    if (ensure(BlackboardComponent))
    {
        AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject(TEXT("TargetActor")));
        if (TargetActor) {
            AAIController* AIController = Cast<AAIController>(OwnerComp.GetAIOwner());
            if (ensure(AIController))
            {
                APawn* AIPawn = AIController->GetPawn();
                if (ensure(AIPawn))
                {
                    float Distance = FVector::Distance(AIPawn->GetActorLocation(), TargetActor->GetActorLocation());
                    bool bWithInRange = Distance < 1000.f;

                    bool bHasLos = false;
                    if (bWithInRange)
                    {
                        // check if we have line of sight to the target
                        bHasLos = AIController->LineOfSightTo(TargetActor);
                    }

                    BlackboardComponent->SetValueAsBool(AttackRangeKey.SelectedKeyName, bWithInRange && bHasLos);
                }
            }
        }
    }
}


