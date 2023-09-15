// Fill out your copyright notice in the Description page of Project Settings.


#include "QMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "QAttributeComponent.h"
#include "QGamePlayFunctionLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "QActionComponent.h"
#include "QActionEffect.h"


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
		UQActionComponent* ActionComp = Cast<UQActionComponent>(
			OtherActor->GetComponentByClass(UQActionComponent::StaticClass()));

		// Parry
		if (ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
		{
			MoveComp->Velocity = -MoveComp->Velocity;

			SetInstigator(Cast<APawn>(OtherActor));  // 设置 Instigator 为对方，这样就可以造成伤害了
			return;
		}

		// Damage
		if (UQGamePlayFunctionLibrary::ApplyDirectionalDamage(this->GetInstigator(), 
			OtherActor, DamageAmount, SweepResult))
		{
			Explode();

			if (ActionComp && HasAuthority()) 
			{
				ActionComp->AddAction(GetInstigator(), BurningActionClass);
			}
		}
	}
}

