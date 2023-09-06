// Fill out your copyright notice in the Description page of Project Settings.


#include "QMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "QAttributeComponent.h"
#include "QGamePlayFunctionLibrary.h"

// Sets default values
AQMagicProjectile::AQMagicProjectile()
{
 	SphereComp->SetSphereRadius(20.0f);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AQMagicProjectile::OnActorOverlap);

	InitialLifeSpan = 10.0f;  // 设置生命周期
	DamageAmount = 10.0f;
	MoveComp->InitialSpeed = 2000.0f;
}

void AQMagicProjectile::OnActorOverlap(UPrimitiveComponent *OverlappedComp, 
	AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		// // 获取角色的属性组件
		// // UQAttributeComponent::StaticClass() 返回的是 UQAttributeComponent 类的 UClass 对象
		// UQAttributeComponent* AttributeComp = Cast<UQAttributeComponent>(
		// 	OtherActor->GetComponentByClass(UQAttributeComponent::StaticClass()));
		// if (AttributeComp) 
		// {
		// 	// 对角色的生命值进行修改
		// 	AttributeComp->ApplyHealthChange(this->GetInstigator(), -DamageAmount);
		// 	// Destroy()
		// 	Explode();
		// }

		if (UQGamePlayFunctionLibrary::ApplyDirectionalDamage(this->GetInstigator(), 
			OtherActor, DamageAmount, SweepResult))
		{
			Explode();
		}
	}
}

