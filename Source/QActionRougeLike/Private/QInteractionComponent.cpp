// Fill out your copyright notice in the Description page of Project Settings.


#include "QInteractionComponent.h"
#include "QGamePlayInterface.h"
#include "DrawDebugHelpers.h"
#include "QWorldUserWidget.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(
	TEXT("Q.DebugDrawInteraction"), false, TEXT("Enable/Disable debug draw for interaction"), ECVF_Cheat);

UQInteractionComponent::UQInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;

	TraceDistance = 500.0f;
	TraceRadius = 20.0f;
	CollisionChannel = ECC_WorldDynamic;
}

void UQInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, 
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FindBestInteractable();
}

void UQInteractionComponent::FindBestInteractable()
{
	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();

	// Create a collision query params object
	FCollisionObjectQueryParams ObjectQueryParams;
	// ECC_WorldDynamic is the default collision channel for actors
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

	AActor* Owner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;
	Owner->GetActorEyesViewPoint(EyeLocation, EyeRotation);  // Get the location and rotation of the owner
	FVector End = EyeLocation + EyeRotation.Vector() * TraceDistance;  // Get the end of the line trace

	TArray<FHitResult> HitResults;

	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(TraceRadius);

	// Sweep a sphere from the owner's eyes to 1000 units in front of the owner
	bool bBlockHit = GetWorld()->SweepMultiByObjectType(HitResults, EyeLocation, End, 
		FQuat::Identity, ObjectQueryParams, CollisionShape);
	
	FColor DebugColor = bBlockHit ? FColor::Green : FColor::Red;

	// Clear the focused actor
	FocusedActor = nullptr;

	for (FHitResult HitResult : HitResults)
	{
		if (bDebugDraw)
		{
			DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, TraceRadius, 16, DebugColor, false, 2.0f);
		}

		AActor* HitActor = HitResult.GetActor();  // Get the actor that was hit
		if (HitActor)
		{
			// Check if the actor implements the interface (UQGamePlayInterface
			if (HitActor->Implements<UQGamePlayInterface>())  
			{
				FocusedActor = HitActor;
				break;
			}
		}
	}

	if (FocusedActor)
	{
		if (InteractionWidgetInstance == nullptr && ensure(InteractionWidgetClass))
		{
			InteractionWidgetInstance = CreateWidget<UQWorldUserWidget>(GetWorld(), InteractionWidgetClass);
		}

		if (InteractionWidgetInstance)
		{
			InteractionWidgetInstance->AttachedActor = FocusedActor;

			if (!InteractionWidgetInstance->IsInViewport())
			{
				InteractionWidgetInstance->AddToViewport();
			}
		}
	}
	else
	{
		if (InteractionWidgetInstance)
		{
			InteractionWidgetInstance->RemoveFromParent();
		}
	}

	// Draw a debug line
	if (bDebugDraw)
	{
		DrawDebugLine(GetWorld(), EyeLocation, End, DebugColor, false, 2.0f);
	}
}


void UQInteractionComponent::PrimaryInteract()
{
	if (FocusedActor == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("No interactable found"));
		return;
	}

	// Call the Iteract_Implementation function
	APawn* Pawn = Cast<APawn>(GetOwner());  // Cast the owner to a pawn
	// Call the Interact_Implementation function
	IQGamePlayInterface::Execute_Interact(FocusedActor, Pawn);  
}


