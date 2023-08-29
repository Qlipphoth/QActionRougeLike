// Fill out your copyright notice in the Description page of Project Settings.


#include "QItemChest.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
AQItemChest::AQItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateAbstractDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;

	LidMesh = CreateAbstractDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(BaseMesh);

	TargetPitch = 110.0f;
}

void AQItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	LidMesh->SetRelativeRotation(FRotator(TargetPitch, 0.0f, 0.0f));  // Rotate the lid
}

// Called when the game starts or when spawned
void AQItemChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AQItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

