// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QInteractionComponent.generated.h"

class UQWorldUserWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QACTIONROUGELIKE_API UQInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	void PrimaryInteract();
		
protected:

	UFUNCTION(Server, Reliable)
	void ServerInteract(AActor* InFocus);

	void FindBestInteractable();

	UPROPERTY()
	AActor* FocusedActor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trace")
	float TraceDistance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trace")
	float TraceRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trace")
	TEnumAsByte<ECollisionChannel> CollisionChannel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UQWorldUserWidget> InteractionWidgetClass;

	UPROPERTY()
	UQWorldUserWidget* InteractionWidgetInstance;

public:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, 
		FActorComponentTickFunction* ThisTickFunction) override;

	UQInteractionComponent();
};
