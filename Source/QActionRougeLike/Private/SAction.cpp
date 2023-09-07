// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"

void USAction::StartAction_Implementation(AActor *Instigator)
{
    UE_LOG(LogTemp, Warning, TEXT("Action %s started"), *ActionName.ToString());
}

void USAction::StopAction_Implementation(AActor *Instigator)
{
    UE_LOG(LogTemp, Warning, TEXT("Action %s stopped"), *ActionName.ToString());
}

UWorld *USAction::GetWorld() const
{
    // Outer is set when creating action via NewObject<T>
    UActorComponent *Component = Cast<UActorComponent>(GetOuter());
    if (Component)
    {
        return Component->GetWorld();
    }
    return nullptr;
}