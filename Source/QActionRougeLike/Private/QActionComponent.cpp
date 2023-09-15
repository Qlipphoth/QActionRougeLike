// Fill out your copyright notice in the Description page of Project Settings.


#include "QActionComponent.h"
#include "SAction.h"
#include "../QActionRougeLike.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"

UQActionComponent::UQActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

// Called when the game starts
void UQActionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->HasAuthority())
	{
		for (TSubclassOf<USAction> ActionClass : DefaultActions)
		{
			AddAction(GetOwner(), ActionClass);
		}
	}
}

void UQActionComponent::AddAction(AActor* Instigator, TSubclassOf<USAction> ActionClass)
{
	// Skip for client
	if (!GetOwner()->HasAuthority()) {
		UE_LOG(LogTemp, Warning, TEXT("Client can't add action"));
		return;
	}

	if (ensure(ActionClass))
	{
		USAction* NewAction = NewObject<USAction>(GetOwner(), ActionClass);
		if (ensure(NewAction))
		{
			NewAction->Initialize(this);
			
			Actions.Add(NewAction);

			if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator)))
			{
				NewAction->StartAction(Instigator);
			}
		}
	}
}

void UQActionComponent::RemoveAction(USAction *ActionToRemove)
{
	if (ensure(ActionToRemove && !ActionToRemove->ISRunning()))
	{
		Actions.Remove(ActionToRemove);
	}
}

bool UQActionComponent::StartActionByName(AActor *Instigator, FName ActionName)
{
	for (USAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (!Action->CanStart(Instigator)) {
				FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActionName.ToString() + " Can't Start";
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, DebugMsg);
				continue;
			}

			// Is Client
			if (!GetOwner()->HasAuthority())
			{
				// 让 server 端的 client 角色执行同样的命令
				ServerStartAction(Instigator, ActionName);
			}

			Action->StartAction(Instigator);
			return true;
		}
	}
	return false;
}

bool UQActionComponent::StopActionByName(AActor *Instigator, FName ActionName)
{
	for (USAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (Action->ISRunning())
			{

				// Is Client
				if (!GetOwner()->HasAuthority())
				{
					// 让 server 端的 client 角色执行同样的命令
					ServerStopAction(Instigator, ActionName);
				}

				Action->StopAction(Instigator);
				return true;
			}
		}
	}
	return false;
}

void UQActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	// GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Yellow, DebugMsg);

	// Draw All Actions
	for (USAction* Action : Actions)
	{
		FColor TextColor = Action->ISRunning() ? FColor::Blue : FColor::White;

		FString ActionMsg = FString::Printf(TEXT("[%s] Action : %s"), 
			*GetNameSafe(GetOwner()), *GetNameSafe(Action));
		LogOnScreen(GetOwner(), ActionMsg, TextColor, 0.0f);
	}

}

void UQActionComponent::ServerStartAction_Implementation(AActor *Instigator, FName ActionName)
{
	// GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "ServerStartAction_Implementation");
	StartActionByName(Instigator, ActionName);
}

void UQActionComponent::ServerStopAction_Implementation(AActor *Instigator, FName ActionName)
{
	StopActionByName(Instigator, ActionName);
}

void UQActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// DOREPLIFETIME(UQActionComponent, Actions);
	DOREPLIFETIME(UQActionComponent, Actions);
}

bool UQActionComponent::ReplicateSubobjects(UActorChannel *Channel, FOutBunch *Bunch, FReplicationFlags *RepFlags)
{
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (USAction* Action : Actions)
	{
		if (Action)
		{
			bWroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}

	return bWroteSomething;
}