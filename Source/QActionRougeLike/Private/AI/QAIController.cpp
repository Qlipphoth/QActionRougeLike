// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/QAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void AQAIController::BeginPlay()
{
    Super::BeginPlay();

    if (BehaviorTree)
    {
        RunBehaviorTree(BehaviorTree);
    }

    // WorldContextObject is the object that we want to get the player pawn from
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
    if (PlayerPawn) {
        // GetBlackboardComponent()->SetValueAsVector(TEXT("MoveToLocation"), PlayerPawn->GetActorLocation());
        GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), PlayerPawn);
    }

}