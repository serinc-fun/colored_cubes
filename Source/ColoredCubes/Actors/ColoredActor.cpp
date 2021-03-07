
#include "ColoredActor.h"

AColoredActor::AColoredActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AColoredActor::SetColor(FLinearColor InColor)
{
	if (IsValid(MID))
	{
		MID->SetVectorParameterValue(ColorParameter, InColor);
	}
}

void AColoredActor::BeginPlay()
{
	Super::BeginPlay();

	if (auto foundMesh = Cast<UMeshComponent>(GetComponentByClass(UMeshComponent::StaticClass())))
	{
		MID = foundMesh->CreateAndSetMaterialInstanceDynamic(0);
		SetColor(DefaultColor);
	}
}
