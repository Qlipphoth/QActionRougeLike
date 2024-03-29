// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "QPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPawnChanged, APawn*, NewPawn);

/**
 * 
 */
UCLASS()
class QACTIONROUGELIKE_API AQPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnPawnChanged OnPawnChanged;

	virtual void SetPawn(APawn* InPawn) override;
};
