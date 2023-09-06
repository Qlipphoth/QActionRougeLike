// Fill out your copyright notice in the Description page of Project Settings.


#include "QAttributeComponent.h"
#include "QGameModeBase.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(
	TEXT("Q.DamageMultiplier"), 1.0f, TEXT("Global damage modifier"), ECVF_Cheat);

// Sets default values for this component's properties
UQAttributeComponent::UQAttributeComponent()
{
	HealthMax = 100.0f;
	Health = HealthMax;
}

bool UQAttributeComponent::Kill(AActor* Instigator)
{
	return ApplyHealthChange(Instigator, -HealthMax);
}

float UQAttributeComponent::GetHealthMax() const
{
	return HealthMax;
}

bool UQAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool UQAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{	
	// 如果角色不可被伤害，则返回 false
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f)
	{
		return false;
	}

	if (Delta < 0.0f)
	{
		Delta *= CVarDamageMultiplier.GetValueOnGameThread();
	}

	float oldHealth = Health;
	Health = FMath::Clamp(Health + Delta, 0.0f, HealthMax);

	float ActualDelta = Health - oldHealth;

	// 触发生命值变化的委托
	OnHealthChangeDelegate.Broadcast(InstigatorActor, this, Health, Delta);

	// Died
	if (ActualDelta < 0.0f && Health <= 0.0f)
	{
		AQGameModeBase* GM = GetWorld()->GetAuthGameMode<AQGameModeBase>();
		if (GM)
		{
			GM->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}

	return ActualDelta != 0.0f;
}

UQAttributeComponent* UQAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<UQAttributeComponent>(FromActor->GetComponentByClass(UQAttributeComponent::StaticClass()));
	}

	return nullptr;
}

bool UQAttributeComponent::IsActorAlive(AActor* Actor)
{
	UQAttributeComponent* Attributes = GetAttributes(Actor);
	if (Attributes)
	{
		return Attributes->IsAlive();
	}

	return false;
}