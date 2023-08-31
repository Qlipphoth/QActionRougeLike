// Fill out your copyright notice in the Description page of Project Settings.


#include "QAttributeComponent.h"

// Sets default values for this component's properties
UQAttributeComponent::UQAttributeComponent()
{
	Health = 100.0f;

}

bool UQAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;
	return true;
}
