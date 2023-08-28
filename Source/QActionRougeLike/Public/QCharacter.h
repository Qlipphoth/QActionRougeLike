// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "QCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class QACTIONROUGELIKE_API AQCharacter : public ACharacter
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere)
	// 子弹，可在编辑器中赋值，类似于 Unity 中的 public GameObject Projectile;
	TSubclassOf<AActor> ProjectileClass;

public:
	// Sets default values for this character's properties
	AQCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QCharacter")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QCharacter")
	UCameraComponent* CameraComp;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void PrimaryAttack();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
