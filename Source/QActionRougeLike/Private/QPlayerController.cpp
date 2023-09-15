// Fill out your copyright notice in the Description page of Project Settings.


#include "QPlayerController.h"

void AQPlayerController::SetPawn(APawn* InPawn)
{
    Super::SetPawn(InPawn);

    OnPawnChanged.Broadcast(InPawn);
}