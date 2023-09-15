// Fill out your copyright notice in the Description page of Project Settings.


#include "QCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "QInteractionComponent.h"
#include "Animation/AnimMontage.h"
#include "Kismet/KismetMathLibrary.h"
#include "QAttributeComponent.h"
#include "DrawDebugHelpers.h"
#include "QActionComponent.h"

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

	ActionComp = CreateDefaultSubobject<UQActionComponent>("ActionComp");

	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input
	bUseControllerRotationYaw = false;  // We want to control yaw with mouse
}

void AQCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComp->OnHealthChangeDelegate.AddDynamic(this, &AQCharacter::OnHealthChanged);
}

/// @brief 重写获取视角的函数
FVector AQCharacter::GetPawnViewLocation() const
{
    return CameraComp->GetComponentLocation();
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

void AQCharacter::sprintStart()
{
	ActionComp->StartActionByName(this, "Sprint");
}

void AQCharacter::sprintStop()
{
	ActionComp->StopActionByName(this, "Sprint");
}

void AQCharacter::PrimaryAttack()
{
	ActionComp->StartActionByName(this, "PrimaryAttack");
}

void AQCharacter::DashAttack()
{
	ActionComp->StartActionByName(this, "DashAttack");
}

void AQCharacter::BlackHoleAttack()
{
	ActionComp->StartActionByName(this, "BlackHoleAttack");
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
	// 直接使用 ACharacter 的 Jump 函数
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AQCharacter::sprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AQCharacter::sprintStop);


	// IE_Pressed: 按下时触发，相当于 unity 的 GetKeyDown
	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &AQCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("DashAttack", IE_Pressed, this, &AQCharacter::DashAttack);
	PlayerInputComponent->BindAction("BlackHoleAttack", IE_Pressed, this, &AQCharacter::BlackHoleAttack);

	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &AQCharacter::PrimaryInteract);
	
}

void AQCharacter::HealSelf(float HealAmount /*= 100.0f*/)
{
	AttributeComp->ApplyHealthChange(this, HealAmount);
}

void AQCharacter::OnHealthChanged(AActor* InstigatorActor, UQAttributeComponent* OwningComp, 
	float NewHealth, float Delta)
{
	if (NewHealth <= 0.0f && Delta < 0.0f)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);

		SetLifeSpan(10.0f);
	}
}