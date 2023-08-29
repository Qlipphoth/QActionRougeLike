// Fill out your copyright notice in the Description page of Project Settings.


#include "QRedBarrel.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
AQRedBarrel::AQRedBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateAbstractDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;

	MeshComp->SetSimulatePhysics(true);
	// bind the OnHit function to the OnComponentHit event
	MeshComp->OnComponentHit.AddDynamic(this, &AQRedBarrel::OnHit);  

	RadialForceComp = CreateAbstractDefaultSubobject<URadialForceComponent>("RadialForceComp");
	RadialForceComp->SetupAttachment(RootComponent);
	RadialForceComp->Radius = 700.0f;
	RadialForceComp->bImpulseVelChange = true;  // make it so that the impulse is not affected by mass
	RadialForceComp->ForceStrength = 2000.0f;

}

void AQRedBarrel::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    // if (OtherActor && OtherActor != this)
    // {
    //     RadialForceComp->FireImpulse();
    // }
	RadialForceComp->FireImpulse();

	UE_LOG(LogTemp, Log, TEXT("OnActorHit in Explosive Barrel"));

	// %s = string
	// %f = float
	// logs: "OtherActor: MyActor_1, at gametime: 124.4" 
	UE_LOG(LogTemp, Warning, TEXT("OtherActor: %s, at game time: %f"), *GetNameSafe(OtherActor), GetWorld()->TimeSeconds);

	FString CombinedString = FString::Printf(TEXT("Hit at location: %s"), *Hit.ImpactPoint.ToString());
	DrawDebugString(GetWorld(), Hit.ImpactPoint, CombinedString, nullptr, FColor::Green, 2.0f, true);
}

// Called when the game starts or when spawned
void AQRedBarrel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AQRedBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

