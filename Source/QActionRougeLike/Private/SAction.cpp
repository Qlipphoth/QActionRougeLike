// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"
#include "QActionComponent.h"
#include "../QActionRougeLike.h"
#include "Net/UnrealNetwork.h"

void USAction::Initialize(UQActionComponent *NewActionComp)
{
    ActionComp = NewActionComp;
}


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
    // UE_LOG(LogTemp, Warning, TEXT("Action %s started"), *ActionName.ToString());
    LogOnScreen(this, "Action " + ActionName.ToString() + " started", FColor::Green);

    UQActionComponent* comp = GetOwningComponent();
    comp->ActiveGameplayTags.AppendTags(GrantsTags);

    bIsRunning = true;
}

void USAction::StopAction_Implementation(AActor *Instigator)
{
    // UE_LOG(LogTemp, Warning, TEXT("Action %s stopped"), *ActionName.ToString());
    LogOnScreen(this, "Action " + ActionName.ToString() + " stopped", FColor::Red);

    // ensureAlways(bIsRunning);

    UQActionComponent* comp = GetOwningComponent();
    comp->ActiveGameplayTags.RemoveTags(GrantsTags);

    bIsRunning = false;
}

UWorld *USAction::GetWorld() const
{
    // Outer is set when creating action via NewObject<T>
    AActor* owner = Cast<AActor>(GetOuter());
    if (owner)
    {
        return owner->GetWorld();
    }
    return nullptr;
}

UQActionComponent *USAction::GetOwningComponent() const
{
    // return Cast<UQActionComponent>(GetOuter());
    return ActionComp;
}

bool USAction::ISRunning() const
{
    return bIsRunning;
}

void USAction::OnRep_IsRunning()
{
    if (bIsRunning)
    {
        StartAction(nullptr);
    }
    else
    {
        StopAction(nullptr);
    }
}


void USAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Replicate bIsRunning
    DOREPLIFETIME(USAction, bIsRunning);
    DOREPLIFETIME(USAction, ActionComp);
}