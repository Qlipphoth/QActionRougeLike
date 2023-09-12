// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QWorldUserWidget.generated.h"


class USizeBox;

UCLASS()
class QACTIONROUGELIKE_API UQWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// BindWidget is a macro that binds a widget variable to a widget in the widget blueprint
	UPROPERTY(meta = (BindWidget))
	USizeBox* ParentSizeBox;

public:

	UPROPERTY(EditAnywhere, Category = "UI")
	FVector WorldOffset;

	// TODO: expose on spawn
	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (ExposeOnSpawn = true))
	AActor* AttachedActor;

};
