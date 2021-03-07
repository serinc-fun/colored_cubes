
#include "ActorSensingComponent.h"
#include "EngineUtils.h"

UActorSensingComponent::UActorSensingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}

bool UActorSensingComponent::CouldSee(const AActor* InActor) const
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

		return (SelfToOtherDir | MyFacingDir) >= PeripheralVisionCosine;
	}

	return false;
}

FTransform UActorSensingComponent::GetSensorTransform() const
{
	if (auto owner = GetOwner())
	{
		return owner->GetActorTransform();
	}

	return FTransform::Identity;
}

void UActorSensingComponent::SetPeripheralVisionAngle(const float NewPeripheralVisionAngle)
{
	PeripheralVisionAngle = NewPeripheralVisionAngle;
	PeripheralVisionCosine = FMath::Cos(FMath::DegreesToRadians(PeripheralVisionAngle));
}

void UActorSensingComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetPeripheralVisionAngle(PeripheralVisionAngle);
	
	GetWorld()->GetTimerManager().SetTimer(SensingIntervalTimerHandle, this, &UActorSensingComponent::SensingHandle, SensingInterval, true);
}

void UActorSensingComponent::SensingHandle()
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
