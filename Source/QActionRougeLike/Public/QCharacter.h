// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "QCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UQInteractionComponent;
class UAnimMontage;
class UQAttributeComponent;
class UQActionComponent;

UCLASS()
class QACTIONROUGELIKE_API AQCharacter : public ACharacter
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	// 子弹，可在编辑器中赋值，类似于 Unity 中的 public GameObject Projectile;
	TSubclassOf<AActor> PrimaryProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TSubclassOf<AActor> DashProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TSubclassOf<AActor> BlackHoleProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	// 攻击动画，可在编辑器中赋值，类似于 Unity 中的 public AnimationClip AttackAnim;
	UAnimMontage* AttackAnim;

	FTimerHandle TimerHandle_PrimaryAttack;

public:
	// Sets default values for this character's properties
	AQCharacter();

protected:
	// // Called when the game starts or when spawned
	// virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QCharacter")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QCharacter")
	UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QCharacter")
	UQInteractionComponent* InteractionComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QCharacter")
	UQAttributeComponent* AttributeComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QCharacter")
	UQActionComponent* ActionComp;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void SpwanProjectile(TSubclassOf<AActor> ProjectileClass);

	void sprintStart();

	void sprintStop();

	void PrimaryAttack();

	void DashAttack();

	void BlackHoleAttack();

	void PrimaryInteract();

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UQAttributeComponent* OwningComp, 
		float NewHealth, float Delta);

	virtual void PostInitializeComponents() override;

	virtual FVector GetPawnViewLocation() const override;

public:	
	// // Called every frame
	// virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Exec 函数，可在控制台中调用，类似于 Unity 中的 [Command] 函数
	UFUNCTION(Exec)
	void HealSelf(float HealAmount = 100.0f);

};
