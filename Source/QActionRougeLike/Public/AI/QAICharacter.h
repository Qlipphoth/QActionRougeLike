// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "QAICharacter.generated.h"

class UPawnSensingComponent;
class UQAttributeComponent;

UCLASS()
class QACTIONROUGELIKE_API AQAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AQAICharacter();

protected:

	UPROPERTY(VisibleAnywhere, Category = "AI")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UQAttributeComponent* AttributeComp;

	UFUNCTION()
	void OnPawnSeen(APawn* SeenPawn);

	virtual void PostInitializeComponents() override;

};
