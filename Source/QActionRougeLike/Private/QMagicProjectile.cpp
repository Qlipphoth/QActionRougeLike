// Fill out your copyright notice in the Description page of Project Settings.


#include "QMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
AQMagicProjectile::AQMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Projectile");  // 设置碰撞检测的类型
	RootComponent = SphereComp;

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(RootComponent);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 1000.0f;  // 初始速度
	MovementComp->bRotationFollowsVelocity = true;  // 速度改变时，自动旋转
	MovementComp->bInitialVelocityInLocalSpace = true;  // 初始速度使用本地坐标系

}

// Called when the game starts or when spawned
void AQMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AQMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

