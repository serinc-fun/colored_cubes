
#include "SpawnerVolume.h"

#include "Components/BrushComponent.h"

ASpawnerVolume::ASpawnerVolume()
{
	
}

void ASpawnerVolume::BeginPlay()
{
	Super::BeginPlay();

	if (SpawnTargets.Num() > 0)
	{
		auto b = GetBrushComponent()->Bounds;
	
        const float volumeHeight = b.GetBox().Max.Z - b.GetBox().Min.Z;
		while (SpawnTargets.Num() > 0)
		{
			FVector randomLocation = b.Origin;
			randomLocation.X = FMath::Lerp(b.GetBox().Min.X, b.GetBox().Max.X, FMath::FRand());
			randomLocation.Y = FMath::Lerp(b.GetBox().Min.Y, b.GetBox().Max.Y, FMath::FRand());
			randomLocation.Z = b.GetBox().Max.Z;

			TArray<TSubclassOf<AActor>> actors;
			SpawnTargets.GetKeys(actors);

			const auto randomActor = actors[FMath::RandRange(0, actors.Num() - 1)];

			FVector randomActorExtent, randomActorOrigin;
			randomActor.GetDefaultObject()->GetActorBounds(true, randomActorOrigin, randomActorExtent, true);
			
			FHitResult hitResult;
        				
            if (GetWorld()->SweepSingleByChannel(hitResult, randomLocation, randomLocation - FVector(0, 0, volumeHeight), FQuat::Identity, ECC_WorldStatic, FCollisionShape::MakeBox(randomActorExtent)))
            {        	        
                if (SpawnTargets.FindChecked(randomActor) > 0)
                {
        	        if (AllowSpawnOnlyOnActors.Num() == 0 || AllowSpawnOnlyOnActors.Contains(hitResult.GetActor()))
        	        {
        	        	--SpawnTargets.FindChecked(randomActor);
        	        	GetWorld()->SpawnActor<AActor>(randomActor, hitResult.Location, FRotator::ZeroRotator);
        	        }
                }
        	    else
        	    {
        	    	SpawnTargets.Remove(randomActor);
        	    }
            }			
		}
	}
}
