// Fill out your copyright notice in the Description page of Project Settings.


#include "QItemChest.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

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

	SetReplicates(true);
}

void AQItemChest::OnRep_IsOpened()
{
	float CurPitch = bIsOpened ? TargetPitch : 0.0f;
	LidMesh->SetRelativeRotation(FRotator(CurPitch, 0.0f, 0.0f));
}

void AQItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	bIsOpened = !bIsOpened;
	OnRep_IsOpened();  // Server 需要手动调用 OnRep_IsOpened
}

void AQItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);  // Call the base class implementation first

	// c : class
	// v : variable
	DOREPLIFETIME(AQItemChest, bIsOpened);
}
