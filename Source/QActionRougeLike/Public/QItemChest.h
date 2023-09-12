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
	// BlueprintNativeEvent means that this function can be implemented in C++ or in Blueprint
	void Interact_Implementation(APawn* InstigatorPawn);

public:	
	// Sets default values for this actor's properties
	AQItemChest();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QItemChest")
	UStaticMeshComponent* BaseMesh;  // BaseMesh is the root component

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QItemChest")
	UStaticMeshComponent* LidMesh;  // LidMesh is a child of BaseMesh

	// client 会自动调用 OnRep_IsOpened
	UPROPERTY(ReplicatedUsing = "OnRep_IsOpened", VisibleAnywhere, BlueprintReadOnly, Category = "QItemChest")
	bool bIsOpened;

	UFUNCTION()
	void OnRep_IsOpened();

};
