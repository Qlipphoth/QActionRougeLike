// Fill out your copyright notice in the Description page of Project Settings.


#include "QActionComponent.h"
#include "SAction.h"

// Called when the game starts
void UQActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<USAction> ActionClass : DefaultActions)
	{
		AddAction(ActionClass);
	}
	
}

void UQActionComponent::AddAction(TSubclassOf<USAction> ActionClass)
{
	if (ensure(ActionClass))
	{
		USAction* NewAction = NewObject<USAction>(this, ActionClass);
		if (ensure(NewAction))
		{
			Actions.Add(NewAction);
		}
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

	FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Yellow, DebugMsg);
}