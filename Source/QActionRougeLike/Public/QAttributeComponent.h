// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QAttributeComponent.generated.h"


// 声明一个委托，用于角色生命值变化时的回调，类似于 Unity 中的 public delegate void OnHealthChangeDelegate();
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChangeSignature, 
	AActor*, InstigatorActor,
	UQAttributeComponent*, OwningComp, 
	float, NewHealth, 
	float, Delta
);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QACTIONROUGELIKE_API UQAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UQAttributeComponent();

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static UQAttributeComponent* GetAttributes(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category = "Attributes", meta = (DisplayName = "IsAlive"))
	static bool IsActorAlive(AActor* Actor);

protected:

	// EditAnywhere - edit in BP editor and per-instance in level.
	// VisibleAnywhere - 'read-only' in editor and level. (Use for Components)
	// EditDefaultsOnly - hide variable per-instance, edit in BP editor only
	// VisibleDefaultsOnly - 'read-only' access for variable, only in BP editor (uncommon)
	// EditInstanceOnly - allow only editing of instance (eg. when placed in level)
	// --
	// BlueprintReadOnly - read-only in the Blueprint scripting (does not affect 'details'-panel)
	// BlueprintReadWrite - read-write access in Blueprints
	// --
	// Category = "" - display only for detail panels and blueprint context menu.

	// EditDefaultsOnly : can only be edited in the blueprint editor
	// BlueprintReadOnly : can only be read in the blueprint editor
	// Category : the category in the blueprint editor
	// meta : additional information
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes", meta = (ClampMin = 0.0f))
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes", meta = (ClampMin = 0.0f))
	float HealthMax;

	// TODO:
	// ReplicatedUsing : OnStateChanged
	// MultiCast : transient events, dont change state

	UFUNCTION(NetMulticast, Reliable)
	void MulticastHealthChanged(AActor* InstigatorActor, float NewHealth, float Delta);

public:	
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool Kill(AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetHealthMax() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(AActor* Instigator, float Delta);	

	// BlueprintAssignable : can be assigned in the blueprint editor
	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnHealthChangeSignature OnHealthChangeDelegate;	
};
