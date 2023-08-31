// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QMagicProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;

UCLASS()
class QACTIONROUGELIKE_API AQMagicProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQMagicProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 用于碰撞检测的球体组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QMagicProjectile")
	USphereComponent* SphereComp;

	// 用于移动的组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QMagicProjectile")
	UProjectileMovementComponent* MovementComp;

	// 用于播放特效的组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QMagicProjectile")
	UParticleSystemComponent* EffectComp;

	// 碰撞检测回调函数
	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
		bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
