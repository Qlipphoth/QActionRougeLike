// Fill out your copyright notice in the Description page of Project Settings.


#include "QAttributeComponent.h"
#include "QGameModeBase.h"
#include "Net/UnrealNetwork.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(
	TEXT("Q.DamageMultiplier"), 1.0f, TEXT("Global damage modifier"), ECVF_Cheat);

// Sets default values for this component's properties
UQAttributeComponent::UQAttributeComponent()
{
	HealthMax = 100.0f;
	Health = HealthMax;

	SetIsReplicatedByDefault(true);  // TODO: 为什么要设置为 ByDefault
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

	// if (!GetOwner()->HasAuthority())
	// {
	// 	return false;
	// }

	if (Delta < 0.0f)
	{
		Delta *= CVarDamageMultiplier.GetValueOnGameThread();
	}

	float oldHealth = Health;
	float newHealth = FMath::Clamp(Health + Delta, 0.0f, HealthMax);

	float ActualDelta = newHealth - oldHealth;

	// 触发生命值变化的委托
	// OnHealthChangeDelegate.Broadcast(InstigatorActor, this, Health, Delta);
	
	if (GetOwner()->HasAuthority())
	{
		Health = newHealth;

		if (Delta != 0.0f)
		{
			MulticastHealthChanged(InstigatorActor, Health, ActualDelta);
		}

		// Died
		if (ActualDelta < 0.0f && Health <= 0.0f)
		{
			AQGameModeBase* GM = GetWorld()->GetAuthGameMode<AQGameModeBase>();
			if (GM)
			{
				GM->OnActorKilled(GetOwner(), InstigatorActor);
			}
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

// TODO: 这一函数不用在头文件中声明

void UQAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UQAttributeComponent, Health);
	DOREPLIFETIME(UQAttributeComponent, HealthMax);

	// DOREPLIFETIME_CONDITION(UQAttributeComponent, Health, COND_InitialOnly);  // 只在初始化时复制
}

void UQAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	OnHealthChangeDelegate.Broadcast(InstigatorActor, this, NewHealth, Delta);
}