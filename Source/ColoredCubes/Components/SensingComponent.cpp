
#include "SensingComponent.h"
#include "EngineUtils.h"

USensingComponent::USensingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}

bool USensingComponent::CouldSee(const AActor* InActor) const
{
	if (IsValid(InActor))
	{
		FVector const OtherLoc = InActor->GetActorLocation();
		FVector const SensorLoc = GetSensorTransform().GetLocation();
		FVector const SelfToOther = OtherLoc - SensorLoc;

		float const SelfToOtherDistSquared = SelfToOther.SizeSquared();
		if (SelfToOtherDistSquared > FMath::Square(SensingRadius))
		{
			return false;
		}

		if (FMath::Square(FMath::FRand()) * SelfToOtherDistSquared > FMath::Square(0.4f * SensingRadius))
		{
			return false;
		}

		FVector const SelfToOtherDir = SelfToOther.GetSafeNormal();
		FVector const MyFacingDir = GetSensorTransform().GetUnitAxis(EAxis::X);

		return ((SelfToOtherDir | MyFacingDir) >= PeripheralVisionCosine);
	}

	return false;
}

FTransform USensingComponent::GetSensorTransform() const
{
	if (auto owner = GetOwner())
	{
		return owner->GetActorTransform();
	}

	return FTransform::Identity;
}

void USensingComponent::SetPeripheralVisionAngle(const float NewPeripheralVisionAngle)
{
	PeripheralVisionAngle = NewPeripheralVisionAngle;
	PeripheralVisionCosine = FMath::Cos(FMath::DegreesToRadians(PeripheralVisionAngle));
}

void USensingComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetPeripheralVisionAngle(PeripheralVisionAngle);
	
	GetWorld()->GetTimerManager().SetTimer(SensingIntervalTimerHandle, this, &USensingComponent::SensingHandle, SensingInterval, true);
}

void USensingComponent::SensingHandle()
{
	for (AActor* actor : TActorRange<AActor>(GetWorld()))
	{
		if (CouldSee(actor))
		{
			if (SensedActors.AddUnique(actor) != INDEX_NONE)
			{
				OnActorSee.Broadcast(actor);
			}
		}
		else if (SensedActors.Contains(actor))
		{
			SensedActors.Remove(actor);
			OnActorLost.Broadcast(actor);
		}
	}
}
