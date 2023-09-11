// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"
#include "QActionComponent.h"


bool USAction::CanStart_Implementation(AActor *Instigator)
{
    if (ISRunning())
    {
        return false;
    }

    UQActionComponent* comp = GetOwningComponent();
    return comp->ActiveGameplayTags.HasAny(BlockedTags) == false;
}

void USAction::StartAction_Implementation(AActor *Instigator)
{
    UE_LOG(LogTemp, Warning, TEXT("Action %s started"), *ActionName.ToString());

    UQActionComponent* comp = GetOwningComponent();
    comp->ActiveGameplayTags.AppendTags(GrantsTags);

    bIsRunning = true;
}

void USAction::StopAction_Implementation(AActor *Instigator)
{
    UE_LOG(LogTemp, Warning, TEXT("Action %s stopped"), *ActionName.ToString());

    ensureAlways(bIsRunning);

    UQActionComponent* comp = GetOwningComponent();
    comp->ActiveGameplayTags.RemoveTags(GrantsTags);

    bIsRunning = false;
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

UQActionComponent *USAction::GetOwningComponent() const
{
    return Cast<UQActionComponent>(GetOuter());
}

bool USAction::ISRunning() const
{
    return bIsRunning;
}