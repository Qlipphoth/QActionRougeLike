// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"


static void LogOnScreen(UObject* WorldContext, FString Msg, 
    FColor Color = FColor::White, float Duration = 5.0f)
{
    if (!ensure(WorldContext)) return;

    // 用于区分客户端和服务端
    UWorld* World = WorldContext->GetWorld();
    if (!ensure(World)) return;
    FString NetPrefix = World->IsNetMode(NM_Client) ? FString("[Client] ") : FString("[Server] ");

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, Duration, Color, NetPrefix + Msg);
    }
}