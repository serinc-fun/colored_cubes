
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActorSensingComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSensingSignature, AActor*, Actor);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent, DisplayName="SesnsingComponent") )
class COLOREDCUBES_API UActorSensingComponent : public UActorComponent
{
	friend class USenseVisualizerComponent;

	GENERATED_BODY()

public:	

	UActorSensingComponent();

	UFUNCTION(BlueprintPure, Category = "Sensing")
	bool CouldSee(const AActor* InActor) const;

	UFUNCTION(BlueprintPure, Category = "Sensing")
	FTransform GetSensorTransform() const;

	UFUNCTION(BlueprintCallable, Category = "Sensing")
    void SetPeripheralVisionAngle(const float NewPeripheralVisionAngle);

	UFUNCTION(BlueprintCallable, Category = "Sensing")
    FORCEINLINE float GetPeripheralVisionAngle() const { return PeripheralVisionAngle; }

	UFUNCTION(BlueprintCallable, Category = "Sensing")
    FORCEINLINE float GetPeripheralVisionCosine() const { return PeripheralVisionCosine; }

	UPROPERTY(BlueprintAssignable, Category = "Sensing")
	FOnSensingSignature OnActorSee;

	UPROPERTY(BlueprintAssignable, Category = "Sensing")
	FOnSensingSignature OnActorLost;
	
protected:

	//~ Begin UActorComponent Interface.
	virtual void InitializeComponent() override;
	//~ End UActorComponent Interface.
	
	UFUNCTION()
	void SensingHandle();
	
	UPROPERTY(EditAnywhere, Category = "Sensing")
	bool bSenseOverSensed = true;

	UPROPERTY(EditAnywhere, Category = "Sensing")
	float PeripheralVisionAngle = 45.0f;

	UPROPERTY(EditAnywhere, Category = "Sensing")
	float SensingRadius = 10000.0f;
 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sensing")
	float SensingInterval = 0.1f;

	UPROPERTY()
	float PeripheralVisionCosine;

	UPROPERTY()
	TArray<AActor*> SensedActors;

	UPROPERTY()
	FTimerHandle SensingIntervalTimerHandle;
};
