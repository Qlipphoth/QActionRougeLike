// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"  // 由于使用的不是指针，因此不能使用前置声明，需要包含头文件

#include "QActionComponent.generated.h"


class USAction;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QACTIONROUGELIKE_API UQActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer ActiveGameplayTags;

	UFUNCTION(BlueprintCallable, Category = "Action")
	void AddAction(TSubclassOf<USAction> ActionClass);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool StartActionByName(AActor* Instigator, FName ActionName);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool StopActionByName(AActor* Instigator, FName ActionName);

protected:

	UPROPERTY(EditAnywhere, Category = "Action")
	TArray<TSubclassOf<USAction>> DefaultActions;

	UPROPERTY()
	TArray<class USAction*> Actions;

	virtual void BeginPlay() override;

public:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, 
		FActorComponentTickFunction* ThisTickFunction) override;

};
