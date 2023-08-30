// Fill out your copyright notice in the Description page of Project Settings.


#include "QDashProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Sound/SoundCue.h"

AQDashProjectile::AQDashProjectile()
{
    TeleportDelay = 0.2f;
    DetonateDelay = 0.2f;

    MoveComp->InitialSpeed = 6000.0f;
}

void AQDashProjectile::BeginPlay()
{
    Super::BeginPlay();

    // Set timer to teleport the instigator
    // TimerHandle_DelayedDetonate : 用于取消定时器
    // this : 用于指定定时器回调的对象
    // &AQDashProjectile::TeleportInstigator : 用于指定定时器回调的函数
    // TeleportDelay : 用于指定定时器的延迟时间
    GetWorldTimerManager().SetTimer(TimerHandle_DelayedDetonate, this, 
        &AQDashProjectile::Explode, DetonateDelay);
}

void AQDashProjectile::Explode_Implementation()
{
    GetWorldTimerManager().ClearTimer(TimerHandle_DelayedDetonate);
    UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());
    UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
    EffectComp->DeactivateSystem();  // 停止粒子系统

    MoveComp->StopMovementImmediately();  // 停止移动
    SetActorEnableCollision(false);  // 禁用碰撞

    FTimerHandle TimerHandle_DelayedTeleport;
    GetWorldTimerManager().SetTimer(TimerHandle_DelayedTeleport, this, 
        &AQDashProjectile::TeleportInstigator, TeleportDelay);

    // Skip base implementation as it will destroy actor (we need to stay alive a bit longer to finish the 2nd timer)
	//Super::Explode_Implementation();
}

void AQDashProjectile::TeleportInstigator()
{
    AActor* ActorToTeleport = GetInstigator();
    if (ensure(ActorToTeleport))
    {   
        // Keep instigator rotation or it may end up jarring
        // bIsATest : 用于指定是否为测试
        // bNoCheck : 用于指定是否检查碰撞
        ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation(), false, false);

        APawn* InstigatorPawn = Cast<APawn>(ActorToTeleport);
        APlayerController* PC = Cast<APlayerController>(InstigatorPawn->GetInstigatorController());
        if (PC && PC->IsLocalController())
        {
            PC->ClientStartCameraShake(ImpactShake);
        }
    }

    Destroy();
}