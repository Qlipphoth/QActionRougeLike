#include "QTargetDummy.h"
#include "Components/StaticMeshComponent.h"
#include "QAttributeComponent.h"

// Sets default values
AQTargetDummy::AQTargetDummy()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;

	AttributeComp = CreateDefaultSubobject<UQAttributeComponent>("AttributeComp");
	AttributeComp->OnHealthChangeDelegate.AddDynamic(this, &AQTargetDummy::OnHealthChanged);

}

void AQTargetDummy::OnHealthChanged(AActor* InstigatorActor, UQAttributeComponent* OwningComp, 
	float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		MeshComp->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	}
}

