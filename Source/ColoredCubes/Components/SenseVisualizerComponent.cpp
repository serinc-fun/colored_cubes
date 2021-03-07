
#include "SenseVisualizerComponent.h"

#include "SensingComponent.h"
#include "ColoredCubes/Actors/ColoredActor.h"

USenseVisualizerComponent::USenseVisualizerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USenseVisualizerComponent::BeginPlay()
{
	Super::BeginPlay();
	
	RangeColors.KeySort(TLess<float>());
	
	if (auto owner = GetOwner())
	{
		if (auto sensing = owner->FindComponentByClass<USensingComponent>())
		{
			sensing->OnActorSee.AddDynamic(this, &USenseVisualizerComponent::OnActorSee);
			sensing->OnActorLost.AddDynamic(this, &USenseVisualizerComponent::OnActorLost);
		}
	}
}

void USenseVisualizerComponent::OnActorSee(AActor* InActor)
{
	auto owner = GetOwner();
	auto coloredActor = Cast<AColoredActor>(InActor);
	
	if (IsValid(coloredActor) && IsValid(owner))
	{
		const float distance = FVector::Distance(coloredActor->GetActorLocation(), owner->GetActorLocation());
		for (auto pair : RangeColors)
		{
			if (pair.Key < distance)
			{
				coloredActor->SetColor(pair.Value);
			}
		}
	}
}

void USenseVisualizerComponent::OnActorLost(AActor* InActor)
{
	if (auto coloredActor = Cast<AColoredActor>(InActor))
	{
		coloredActor->SetColor(coloredActor->GetDefaultColor());
	}
}
