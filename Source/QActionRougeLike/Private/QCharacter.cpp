// Fill out your copyright notice in the Description page of Project Settings.


#include "QCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "QInteractionComponent.h"
#include "Animation/AnimMontage.h"
#include "Kismet/KismetMathLibrary.h"
#include "QAttributeComponent.h"

// Sets default values
AQCharacter::AQCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<UQInteractionComponent>("InteractionComp");

	AttributeComp = CreateDefaultSubobject<UQAttributeComponent>("AttributeComp");

	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input
	bUseControllerRotationYaw = false;  // We want to control yaw with mouse
}

// Called when the game starts or when spawned
void AQCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AQCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AQCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	// 使用控制器的旋转方向来作为移动的方向
	AddMovementInput(ControlRot.Vector(), Value);
}

void AQCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	// X = Forward (Red)
	// Y = Right (Green)
	// Z = Up (Blue)

	// 获得控制器的右方向
	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	// 使用控制器的右方向来作为移动的方向
	AddMovementInput(RightVector, Value);
}

void AQCharacter::PrimaryAttack()
{
	// 播放动画
	PlayAnimMontage(AttackAnim);
	// 设置定时器，0.2 秒后调用 PrimaryAttack_TimeElapsed
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, 
		&AQCharacter::PrimaryAttack_TimeElapsed, 0.2f);
}

void AQCharacter::PrimaryAttack_TimeElapsed()
{
	// 从骨骼中获取手的位置
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	
	// 设置射线检测的形状
	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(20.0f);

	// 设置射线检测的参数
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);  // 忽略玩家角色

	// 设置射线检测的参数
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

	FVector TraceStart = GetPawnViewLocation();
	// endpoint far into the look-at distance (not too far, still adjust somewhat towards crosshair on a miss)
	FVector TraceEnd = TraceStart + GetControlRotation().Vector() * 5000.0f;

	FHitResult HitResult;
	// 射线检测
	if (GetWorld()->SweepSingleByObjectType(HitResult, TraceStart, TraceEnd, 
		FQuat::Identity, ObjectQueryParams, CollisionShape, QueryParams))
	{
		TraceEnd = HitResult.ImpactPoint;
	}

	// 生成子弹的旋转方向
	FRotator ProjRotation = UKismetMathLibrary::FindLookAtRotation(HandLocation, TraceEnd);
	// 生成子弹的位置
	FTransform SpawnTransform = FTransform(ProjRotation, HandLocation);

	FActorSpawnParameters SpawnParams;
	// AlwaysSpawn: 如果碰撞到其他物体，也会生成
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	// 设置生成的子弹的拥有者
	SpawnParams.Instigator = this;

	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTransform, SpawnParams);
}

void AQCharacter::PrimaryInteract()
{
	InteractionComp->PrimaryInteract();
}

// Called to bind functionality to input
void AQCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AQCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AQCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	// IE_Pressed: 按下时触发，相当于 unity 的 GetKeyDown
	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &AQCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &AQCharacter::PrimaryInteract);
	// 直接使用 ACharacter 的 Jump 函数
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

}

