// Fill out your copyright notice in the Description page of Project Settings.


#include "QActionComponent.h"
#include "SAction.h"

// Sets default values for this component's properties
UQActionComponent::UQActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UQActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<USAction> ActionClass : DefaultActions)
	{
		AddAction(ActionClass);
	}
	
}


// Called every frame
void UQActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
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
			Action->StopAction(Instigator);
			return true;
		}
	}
	return false;
}