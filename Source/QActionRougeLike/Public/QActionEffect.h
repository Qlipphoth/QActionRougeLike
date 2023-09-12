// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "QActionEffect.generated.h"

/**
 * 
 */
UCLASS()
class QACTIONROUGELIKE_API UQActionEffect : public USAction
{
	GENERATED_BODY()
	
public:

	void StartAction_Implementation(AActor* InstigatorActor) override;

	void StopAction_Implementation(AActor* InstigatorActor) override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Duration;

	/* Time between 'ticks' to apply effect */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Period;


	FTimerHandle DurationHandle;
	FTimerHandle PeriodHandle;

	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void ExcutePeriodEffect(AActor* InstigatorActor);

public:

	UQActionEffect();
};
