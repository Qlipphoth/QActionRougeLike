// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/QAICharacter.h"
#include "AIController.h"
#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "QAttributeComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "BrainComponent.h"
#include "QAttributeComponent.h"
#include "QWorldUserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
AQAICharacter::AQAICharacter()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;  // AI 控制器自动获取控制权

	AttributeComp = CreateDefaultSubobject<UQAttributeComponent>(TEXT("AttributeComp"));
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	TimeToHitParamName = TEXT("TimeToHit");
}

void AQAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &AQAICharacter::OnPawnSeen);
	AttributeComp->OnHealthChangeDelegate.AddDynamic(this, &AQAICharacter::OnHealthChanged);
}

void AQAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		AIController->GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), NewTarget);	
	}
}

void AQAICharacter::OnPawnSeen(APawn* SeenPawn)
{
	SetTargetActor(SeenPawn);
	DrawDebugString(GetWorld(), GetActorLocation(), "OnSeen Player", 
			nullptr, FColor::Yellow, 4.0f, true);
}

void AQAICharacter::OnHealthChanged(AActor* InstigatorActor, UQAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		if (InstigatorActor != this)
		{
			SetTargetActor(InstigatorActor);
		}

		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);

		// HealthBar
		if (ActivateHealthBar == nullptr)
		{
			ActivateHealthBar = CreateWidget<UQWorldUserWidget>(GetWorld(), HealthBarWidget);
			if (ActivateHealthBar)
			{
				ActivateHealthBar->AttachedActor = this;
				ActivateHealthBar->AddToViewport();
			}
		}

		// Died
		if (NewHealth <= 0.0f)
		{
			// Stop BT
			AAIController* AIController = Cast<AAIController>(GetController());
			if (AIController)
			{
				AIController->BrainComponent->StopLogic("Dead");
			}

			// ragdoll
			GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->bBlendPhysics = true;

			// remove capsule
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();

			// set life span
			SetLifeSpan(10.0f);
		}
	}
}
