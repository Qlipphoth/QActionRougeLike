// Fill out your copyright notice in the Description page of Project Settings.


#include "QWorldUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"


void UQWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (!IsValid(AttachedActor))
    {
        RemoveFromParent();
        UE_LOG(LogTemp, Warning, TEXT("RemoveFromParent"));
        return;
    }

    FVector2D ScreenPosition;
    if (UGameplayStatics::ProjectWorldToScreen(
        GetOwningPlayer(), AttachedActor->GetActorLocation() + WorldOffset, ScreenPosition))
    {
        float scale = UWidgetLayoutLibrary::GetViewportScale(this);
        ScreenPosition /= scale;

        if (ParentSizeBox)
        {
            ParentSizeBox->SetRenderTranslation(ScreenPosition);
        }

    }
}