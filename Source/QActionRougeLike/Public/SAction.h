// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "SAction.generated.h"

class UWorld;
class UQActionComponent;

// Blueprintable : can be created in blueprint

UCLASS(Blueprintable)
class QACTIONROUGELIKE_API USAction : public UObject
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(Replicated)
	UQActionComponent* ActionComp;

	UFUNCTION(BlueprintCallable, Category = "Action")
	UQActionComponent* GetOwningComponent() const;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	UPROPERTY(ReplicatedUsing = "OnRep_IsRunning")
	bool bIsRunning;

	UFUNCTION()
	void OnRep_IsRunning();

public:

	void Initialize(UQActionComponent* NewActionComp);

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart;

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool ISRunning() const;

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(class AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(class AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void StopAction(class AActor* Instigator);

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	UWorld* GetWorld() const override;

	bool IsSupportedForNetworking() const override
	{
		return true;
	}
};
