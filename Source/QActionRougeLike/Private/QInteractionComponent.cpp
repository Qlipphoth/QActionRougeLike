// Fill out your copyright notice in the Description page of Project Settings.


#include "QInteractionComponent.h"
#include "QGamePlayInterface.h"
#include "DrawDebugHelpers.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(
	TEXT("Q.DebugDrawInteraction"), false, TEXT("Enable/Disable debug draw for interaction"), ECVF_Cheat);

// Sets default values for this component's properties
UQInteractionComponent::UQInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UQInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UQInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UQInteractionComponent::PrimaryInteract()
{
	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();

	// Create a collision query params object
	FCollisionObjectQueryParams ObjectQueryParams;
	// ECC_WorldDynamic is the default collision channel for actors
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	AActor* Owner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;
	Owner->GetActorEyesViewPoint(EyeLocation, EyeRotation);  // Get the location and rotation of the owner
	FVector End = EyeLocation + EyeRotation.Vector() * 1000.0f;  // Get the end of the line trace

	TArray<FHitResult> HitResults;
	float Radius = 30.0f;

	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(Radius);

	// Sweep a sphere from the owner's eyes to 1000 units in front of the owner
	bool bBlockHit = GetWorld()->SweepMultiByObjectType(HitResults, EyeLocation, End, 
		FQuat::Identity, ObjectQueryParams, CollisionShape);
	
	FColor DebugColor = bBlockHit ? FColor::Green : FColor::Red;

	for (FHitResult HitResult : HitResults)
	{
		AActor* HitActor = HitResult.GetActor();  // Get the actor that was hit
		if (HitActor)
		{
			// Check if the actor implements the interface (UQGamePlayInterface
			if (HitActor->Implements<UQGamePlayInterface>())  
			{
				// Call the Iteract_Implementation function
				APawn* Pawn = Cast<APawn>(Owner);  // Cast the owner to a pawn
				// Call the Interact_Implementation function
				IQGamePlayInterface::Execute_Interact(HitActor, Pawn);  
				
				if (bDebugDraw)
				{
					DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, Radius, 16, DebugColor, false, 2.0f);
				}
	
				break;
			}
		}
		
	}

	// Draw a debug line
	if (bDebugDraw)
	{
		DrawDebugLine(GetWorld(), EyeLocation, End, DebugColor, false, 2.0f);
	}
}