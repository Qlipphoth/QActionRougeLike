// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "QGamePlayInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UQGamePlayInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class QACTIONROUGELIKE_API IQGamePlayInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	// BlueprintNativeEvent means that this function can be implemented in C++ or in Blueprint
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GamePlayInterface")
	void Interact(APawn* InstigatorPawn);

};
