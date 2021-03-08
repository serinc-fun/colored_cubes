
#include "SenseVisualizerComponent.h"


#include "DrawDebugHelpers.h"
#include "ActorSensingComponent.h"
#include "Camera/CameraComponent.h"
#include "ColoredCubes/Actors/ColoredActor.h"

USenseVisualizerComponent::USenseVisualizerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USenseVisualizerComponent::TickComponent(float DeltaTime, ELevelTick TickType,	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bVisualizeFOV)
	{
		if (auto owner = GetOwner())
		{
			if (IsValid(SensingComponent) && IsValid(CameraComponent))
			{
				const auto location = owner->GetActorTransform().GetLocation();
				const auto direction = owner->GetActorTransform().GetUnitAxis(EAxis::X);

				// Sensing
				const auto senseView = FMath::DegreesToRadians(SensingComponent->PeripheralVisionAngle);				
				DrawDebugCone
                (
                    GetWorld(),
                    location,
                    direction,
                    VisualizeFOVRange,
                    senseView,
                    senseView,
                    8,
                    FColorList::OrangeRed,
                    true,
                    DeltaTime,
                    SDPG_Foreground,
                    8
                );
				
				// Camera
				const float cameraView = FMath::DegreesToRadians(CameraComponent->FieldOfView / 2.0f);				
				DrawDebugCone
                (
                    GetWorld(),
                    location,
                    direction,
                    VisualizeFOVRange,
                    cameraView,
                    cameraView,
                    8,
                    FColorList::LimeGreen,
                    true,
                    DeltaTime,
                    SDPG_Foreground,
                    4
                );
			}
		}
	}
}

void USenseVisualizerComponent::BeginPlay()
{
	Super::BeginPlay();
	
	RangeColors.KeySort(TLess<float>());
	
	if (auto owner = GetOwner())
	{
		SensingComponent = owner->FindComponentByClass<UActorSensingComponent>();
		if (IsValid(SensingComponent))
		{
			float viewAngle = 45.0f;
			
			CameraComponent = owner->FindComponentByClass<UCameraComponent>();
			if (IsValid(CameraComponent))
			{
				viewAngle = (CameraComponent->FieldOfView / 2.0f) + AdditionalViewAngle;
			}

			SensingComponent->SetPeripheralVisionAngle(viewAngle);
			SensingComponent->OnActorSee.AddDynamic(this, &USenseVisualizerComponent::OnActorSee);
			SensingComponent->OnActorLost.AddDynamic(this, &USenseVisualizerComponent::OnActorLost);
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
