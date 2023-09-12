// Fill out your copyright notice in the Description page of Project Settings.


#include "QActionEffect.h"
#include "QActionComponent.h"

UQActionEffect::UQActionEffect()
{
    bAutoStart = true;
}

void UQActionEffect::StartAction_Implementation(AActor *InstigatorActor)
{
    Super::StartAction_Implementation(InstigatorActor);
    
    if (Duration > 0.0f) {
        FTimerDelegate TimerDelegate;
        TimerDelegate.BindUFunction(this, FName("StopAction"), InstigatorActor);

        GetWorld()->GetTimerManager().SetTimer(DurationHandle, TimerDelegate, Duration, false);  // 不为 loop
    }

    if (Period > 0.0f) {
        FTimerDelegate TimerDelegate;
        TimerDelegate.BindUFunction(this, FName("ExcutePeriodEffect"), InstigatorActor);

        GetWorld()->GetTimerManager().SetTimer(PeriodHandle, TimerDelegate, Period, true);  // 为 loop
    }

}

void UQActionEffect::StopAction_Implementation(AActor *InstigatorActor)
{
    // 确保能够执行最后一次
    if (GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER) {
        ExcutePeriodEffect(InstigatorActor);
    }

    Super::StopAction_Implementation(InstigatorActor);

    GetWorld()->GetTimerManager().ClearTimer(DurationHandle);
    GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);

    UQActionComponent* Comp = GetOwningComponent();
    if (Comp) {
        Comp->RemoveAction(this);
    }
}

void UQActionEffect::ExcutePeriodEffect_Implementation(AActor *InstigatorActor)
{
    // 
    // UE_LOG(LogTemp, Warning, TEXT("ExcutePeriodEffect"));
}