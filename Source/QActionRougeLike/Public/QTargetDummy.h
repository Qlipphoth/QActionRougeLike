// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QTargetDummy.generated.h"

class UStaticMeshComponent;
class UQAttributeComponent;

UCLASS()
class QACTIONROUGELIKE_API AQTargetDummy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQTargetDummy();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QTargetDummy")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QTargetDummy")
	UQAttributeComponent* AttributeComp;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UQAttributeComponent* OwningComp, 
		float NewHealth, float Delta);


};
