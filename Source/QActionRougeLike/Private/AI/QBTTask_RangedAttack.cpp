// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/QBTTask_RangedAttack.h"
#include "AI/QAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type UQBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    NodeName = TEXT("Ranged Attack");
    UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
    AAIController* MyController = OwnerComp.GetAIOwner();
    if (ensure(MyController)) {
        ACharacter* MyCharacter = Cast<ACharacter>(MyController->GetPawn());
        if (MyCharacter == nullptr) return EBTNodeResult::Failed;

        FVector MuzzleLocation = MyCharacter->GetMesh()->GetSocketLocation(TEXT("Muzzle_01"));

        AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject(TEXT("TargetActor")));
        if (TargetActor == nullptr) return EBTNodeResult::Failed;

        FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
        FRotator Rotation = Direction.Rotation();

        FActorSpawnParameters SpawnParameters;
        SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        AActor* Projectile = GetWorld()->SpawnActor<AActor>(
            ProjectileClass, MuzzleLocation, Rotation, SpawnParameters);

        return Projectile ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
    }
    return EBTNodeResult::Failed;
}