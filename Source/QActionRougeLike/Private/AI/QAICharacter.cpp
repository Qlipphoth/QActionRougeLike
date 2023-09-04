// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/QAICharacter.h"
#include "AIController.h"
#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "QAttributeComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
AQAICharacter::AQAICharacter()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;  // AI 控制器自动获取控制权

	AttributeComp = CreateDefaultSubobject<UQAttributeComponent>(TEXT("AttributeComp"));

}

void AQAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &AQAICharacter::OnPawnSeen);
}

void AQAICharacter::OnPawnSeen(APawn* SeenPawn)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
		BlackboardComp->SetValueAsObject(TEXT("TargetActor"), SeenPawn);

		DrawDebugString(GetWorld(), GetActorLocation(), "OnSeen Player", 
			nullptr, FColor::Yellow, 4.0f, true);
	}
}

