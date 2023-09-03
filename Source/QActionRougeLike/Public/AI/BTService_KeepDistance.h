// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_KeepDistance.generated.h"

/**
 * 
 */
UCLASS()
class QACTIONROUGELIKE_API UBTService_KeepDistance : public UBTService
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector bIsInRangeKey;

	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector bIsOutRangeKey;

	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector bKeepWait;

	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector RunDirectionKey;

	// TickNode : Called every frame while this is active
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
