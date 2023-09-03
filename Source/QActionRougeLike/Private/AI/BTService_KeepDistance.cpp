// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_KeepDistance.h"
#include "AI/QAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UBTService_KeepDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();

    if (ensure(BlackboardComponent)) {
        AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject(TargetActorKey.SelectedKeyName));
        BlackboardComponent->SetValueAsVector(RunDirectionKey.SelectedKeyName, 
            OwnerComp.GetOwner()->GetActorLocation() - TargetActor->GetActorLocation());

        if (TargetActor) {
            AAIController* AIController = Cast<AAIController>(OwnerComp.GetAIOwner());
            if (ensure(AIController)) {
                APawn* AIPawn = AIController->GetPawn();
                if (ensure(AIPawn)) {
                    float Distance = FVector::Distance(AIPawn->GetActorLocation(), TargetActor->GetActorLocation());
                    bool bInRange = (500.0f - Distance) > 50.f;
                    bool bOutRange = (Distance - 500.0f) > 50.f;
                    // check if we have line of sight to the target
                    bool bHasLos = AIController->LineOfSightTo(TargetActor);
                    
                    BlackboardComponent->SetValueAsBool(bIsInRangeKey.SelectedKeyName, bInRange && bHasLos);
                    BlackboardComponent->SetValueAsBool(bIsOutRangeKey.SelectedKeyName, bOutRange);
                    BlackboardComponent->SetValueAsBool(bKeepWait.SelectedKeyName, !bInRange && !bOutRange);
                }
            }
        }
    }

}