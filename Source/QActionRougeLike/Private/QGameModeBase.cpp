// Fill out your copyright notice in the Description page of Project Settings.


#include "QGameModeBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "AI/QAICharacter.h"
#include "QAttributeComponent.h"
#include "EngineUtils.h"


AQGameModeBase::AQGameModeBase()
{
    SpawnInterval = 2.0f;
}

void AQGameModeBase::StartPlay()
{
    Super::StartPlay();  // Call the base class version of StartPlay first.

    GetWorldTimerManager().SetTimer(
        TimerHandle_SpawnBots, this, &AQGameModeBase::SpawnBotTimerElapsed, SpawnInterval, true);
}

void AQGameModeBase::SpawnBotTimerElapsed()
{
    UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(
        this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
    if (ensure(QueryInstance)) {
        QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AQGameModeBase::OnQueryCompleted);
    }
}

int AQGameModeBase::CountOfBots()
{
    int Count = 0;
    for (TActorIterator<AQAICharacter> It(GetWorld()); It; ++It)
    {
        AQAICharacter* Bot = *It;
        UQAttributeComponent* AttributeComp =  Cast<UQAttributeComponent>(
            Bot->GetComponentByClass(UQAttributeComponent::StaticClass()));
        if (AttributeComp && AttributeComp->IsAlive()) {
            Count++;
        }
    }
    return Count;
}

void AQGameModeBase::OnQueryCompleted(
    UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
    if (QueryStatus == EEnvQueryStatus::Failed)
    {
        UE_LOG(LogTemp, Warning, TEXT("Spawn Bot EQS Failed!"));
        return;
    }

    int Count = CountOfBots();

    // if (Count >= NumOfBotsToSpawn) return;
    if (DifficultyCurve && Count >= DifficultyCurve->GetFloatValue(GetWorld()->GetTimeSeconds()))
        return;

    TArray<FVector> locations = QueryInstance->GetResultsAsLocations();

    if (locations.IsValidIndex(0))
    {
        FActorSpawnParameters SpawnParams;
        // 设置为 AdjustIfPossibleButAlwaysSpawn，如果位置被占用，会自动调整位置
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
        GetWorld()->SpawnActor<AActor>(BotToSpawn, locations[0], FRotator::ZeroRotator, SpawnParams);
    }
}


