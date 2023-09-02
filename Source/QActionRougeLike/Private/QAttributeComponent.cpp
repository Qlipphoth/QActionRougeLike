// Fill out your copyright notice in the Description page of Project Settings.


#include "QAttributeComponent.h"

// Sets default values for this component's properties
UQAttributeComponent::UQAttributeComponent()
{
	Health = 100.0f;

}

bool UQAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool UQAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;

	// 触发生命值变化的委托
	OnHealthChangeDelegate.Broadcast(nullptr, this, Health, Delta);

	return true;
}