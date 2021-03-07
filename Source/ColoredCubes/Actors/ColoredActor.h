// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ColoredActor.generated.h"

UCLASS()
class COLOREDCUBES_API AColoredActor : public AActor
{
	GENERATED_BODY()
	
public:
	
	AColoredActor();

	UFUNCTION(BlueprintCallable, Category = "Colored")
	void SetColor(FLinearColor InColor);
	
protected:

	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category = "Colored")
	FLinearColor DefaultColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, Category = "Colored")
	FName ColorParameter = TEXT("Color");

	UPROPERTY()
	UMaterialInstanceDynamic* MID;
};
