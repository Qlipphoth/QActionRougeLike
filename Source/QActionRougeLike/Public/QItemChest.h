// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QGamePlayInterface.h"
#include "QItemChest.generated.h"


class UStaticMeshComponent;

UCLASS()
class QACTIONROUGELIKE_API AQItemChest : public AActor, public IQGamePlayInterface
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "GamePlayInterface")
	float TargetPitch;

	// _Implementation means that this function is implemented in C++
	void Interact_Implementation(APawn* InstigatorPawn);

public:	
	// Sets default values for this actor's properties
	AQItemChest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "GamePlayInterface")
	UStaticMeshComponent* BaseMesh;  // BaseMesh is the root component

	UPROPERTY(EditAnywhere, Category = "GamePlayInterface")
	UStaticMeshComponent* LidMesh;  // LidMesh is a child of BaseMesh

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
