// Fill out your copyright notice in the Description page of Project Settings.


#include "QMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "QAttributeComponent.h"

// Sets default values
AQMagicProjectile::AQMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Projectile");  // 设置碰撞检测的类型
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AQMagicProjectile::OnActorOverlap);  // 绑定碰撞检测的回调函数
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

void AQMagicProjectile::OnActorOverlap(UPrimitiveComponent *OverlappedComp, 
	AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		// 获取角色的属性组件
		// UQAttributeComponent::StaticClass() 返回的是 UQAttributeComponent 类的 UClass 对象
		UQAttributeComponent* AttributeComp = Cast<UQAttributeComponent>(
			OtherActor->GetComponentByClass(UQAttributeComponent::StaticClass()));
		if (AttributeComp) 
		{
			// 对角色的生命值进行修改
			AttributeComp->ApplyHealthChange(-10.0f);
			Destroy();
		}
	}
}

// Called every frame
void AQMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

