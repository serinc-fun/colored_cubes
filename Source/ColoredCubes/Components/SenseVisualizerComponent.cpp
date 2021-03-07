
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
	
	if (auto owner = GetOwner())
	{
		if (IsValid(SensingComponent) && IsValid(CameraComponent))
		{
			//DrawDebugLine
			//(
				//GetWorld()
			//);
			
			// Sensing
			//DrawDebugAltCone
			//(
			//	GetWorld(),
			//	owner->GetActorLocation(),
			//	owner->GetActorRotation(),
			//	SensingComponent->SensingRadius,
			//	SensingComponent->PeripheralVisionCosine,
			//	SensingComponent->PeripheralVisionCosine,
			//	FColorList::OrangeRed,
			//	true,
			//	DeltaTime,
			//	SDPG_Foreground,
			//	10
			//);
//
			//const float cameraViewCosine = FMath::Cos(FMath::DegreesToRadians(CameraComponent->FieldOfView / 2.0f));
//
			//// Camera
			//DrawDebugAltCone
            //(
            //    GetWorld(),
            //    owner->GetActorLocation(),
            //    owner->GetActorRotation(),
            //    SensingComponent->SensingRadius,
            //    cameraViewCosine,
            //    cameraViewCosine,
            //    FColorList::LimeGreen,
            //    true,
            //    DeltaTime,
            //    SDPG_Foreground,
            //    10
            //);
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
