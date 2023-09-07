// Fill out your copyright notice in the Description page of Project Settings.


#include "Action_ProjectileAttack.h"
#include "Animation/AnimMontage.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "QCharacter.h"
#include "DrawDebugHelpers.h"

UAction_ProjectileAttack::UAction_ProjectileAttack()
{
    AttackAnimDelay = 0.2f;
    HandSocketName = "Muzzle_01";
}

void UAction_ProjectileAttack::StartAction_Implementation(AActor *Instigator)
{
    Super::StartAction_Implementation(Instigator);

    ACharacter* Character = Cast<ACharacter>(Instigator);
    if (Character) 
    {
        Character->PlayAnimMontage(AttackAnim);
        UGameplayStatics::SpawnEmitterAttached(CastingEffect, Character->GetMesh(), HandSocketName, 
            FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true);

        FTimerHandle TimerHandle_AttackDelay;
        FTimerDelegate TimerDelegate;
        TimerDelegate.BindUFunction(this, "AttackDelay_Elapsed", Character);

        GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, TimerDelegate, AttackAnimDelay, false);
    }
}

void UAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter *InstigatorCharacter)
{
    if (ensure(ProjectileClass))
    {
        // 从骨骼中获取手的位置
        FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocketName);
        
        FActorSpawnParameters SpawnParams;
        // AlwaysSpawn: 如果碰撞到其他物体，也会生成
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        // 设置生成的子弹的拥有者
        SpawnParams.Instigator = InstigatorCharacter;

        // 设置射线检测的形状
        FCollisionShape CollisionShape;
        CollisionShape.SetSphere(20.0f);

        // 设置射线检测的参数
        FCollisionQueryParams QueryParams;
        QueryParams.AddIgnoredActor(InstigatorCharacter);  // 忽略玩家角色

        // 设置射线检测的参数
        FCollisionObjectQueryParams ObjectQueryParams;
        ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
        ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
        ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

        FVector TraceStart = InstigatorCharacter->GetPawnViewLocation();
        // endpoint far into the look-at distance (not too far, still adjust somewhat towards crosshair on a miss)
        FVector TraceEnd = TraceStart + InstigatorCharacter->GetControlRotation().Vector() * 5000.0f;

        FHitResult HitResult;
        // 射线检测
        if (GetWorld()->SweepSingleByObjectType(HitResult, TraceStart, TraceEnd, 
            FQuat::Identity, ObjectQueryParams, CollisionShape, QueryParams))
        {
            TraceEnd = HitResult.ImpactPoint;
            DrawDebugSphere(GetWorld(), TraceEnd, 20.0f, 8, FColor::Red, false, 2.0f);
        }

        // 生成子弹的旋转方向
        // FRotator ProjRotation = UKismetMathLibrary::FindLookAtRotation(HandLocation, TraceEnd);
        FRotator ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();

        // 生成子弹的位置
        FTransform SpawnTransform = FTransform(ProjRotation, HandLocation);

        GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTransform, SpawnParams);
    }
    StopAction(InstigatorCharacter);
}




