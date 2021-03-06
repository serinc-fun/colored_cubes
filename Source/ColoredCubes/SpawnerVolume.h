
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Volume.h"
#include "SpawnerVolume.generated.h"

/**
 * 
 */
UCLASS()
class COLOREDCUBES_API ASpawnerVolume : public AVolume
{
	GENERATED_BODY()

public:

	ASpawnerVolume();

	virtual void BeginPlay() override;

protected:

	UPROPERTY(EditAnywhere, Category = Options)
	TMap<TSubclassOf<AActor>, int32> SpawnTargets;

	UPROPERTY(EditAnywhere, Category = Options)
	TArray<AActor*> AllowSpawnOnlyOnActors;
};
