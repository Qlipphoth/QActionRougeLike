// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/QAICharacter.h"

// Sets default values
AQAICharacter::AQAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AQAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AQAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AQAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

