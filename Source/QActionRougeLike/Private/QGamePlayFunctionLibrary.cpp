// Fill out your copyright notice in the Description page of Project Settings.


#include "QGamePlayFunctionLibrary.h"
#include "QAttributeComponent.h"

bool UQGamePlayFunctionLibrary::ApplyDamage(AActor *DamageCauser, AActor *TargetActor, float DamageAmout)
{
    UQAttributeComponent* AttributeComp = UQAttributeComponent::GetAttributes(TargetActor);
    if (AttributeComp) {
        return AttributeComp->ApplyHealthChange(DamageCauser, -DamageAmout);
    }
    return false;
}

bool UQGamePlayFunctionLibrary::ApplyDirectionalDamage(AActor *DamageCauser, AActor *TargetActor, float DamageAmout, const FHitResult &HitResult)
{
    if (ApplyDamage(DamageCauser, TargetActor, DamageAmout)) {
        UPrimitiveComponent* HitComp = HitResult.GetComponent();
        if (HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName)) {
            HitComp->AddImpulseAtLocation(-HitResult.ImpactNormal * 300000.0f, 
            HitResult.ImpactPoint, HitResult.BoneName);
        }
        return true;
    }
    return false;
}