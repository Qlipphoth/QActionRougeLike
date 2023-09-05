// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "QAICharacter.generated.h"

class UPawnSensingComponent;
class UQAttributeComponent;
class UQWorldUserWidget;

UCLASS()
class QACTIONROUGELIKE_API AQAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AQAICharacter();

protected:

	UPROPERTY(VisibleAnywhere, Category = "AI")
	FName TimeToHitParamName;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UQAttributeComponent* AttributeComp;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UQWorldUserWidget> HealthBarWidget;

	UPROPERTY()
	UQWorldUserWidget* ActivateHealthBar;

	UFUNCTION()
	void OnPawnSeen(APawn* SeenPawn);

	UFUNCTION()
	void SetTargetActor(AActor* NewTarget);

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UQAttributeComponent* OwningComp, float NewHealth, float Delta);

};
