// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "QGameModeBase.generated.h"

class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;

UCLASS()
class QACTIONROUGELIKE_API AQGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnInterval;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> BotToSpawn;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	int32 NumOfBotsToSpawn;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyCurve;

	FTimerHandle TimerHandle_SpawnBots;

	UFUNCTION()
	void SpawnBotTimerElapsed();

	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	int CountOfBots();

public:

	AQGameModeBase();

	virtual void StartPlay() override;

	UFUNCTION(Exec)
	void SlayAll();
};
