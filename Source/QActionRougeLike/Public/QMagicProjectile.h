// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QProjectileBase.h"
#include "GameplayTagContainer.h"
#include "QMagicProjectile.generated.h"

UCLASS()
class QACTIONROUGELIKE_API AQMagicProjectile : public AQProjectileBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQMagicProjectile();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FGameplayTag ParryTag;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DamageAmount;

	// 碰撞检测回调函数
	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
		bool bFromSweep, const FHitResult& SweepResult);

};
