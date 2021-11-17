// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingBase.h"
#include "ProducingBuilding.generated.h"

/**
 * 
 */
UCLASS()
class TYCOONBUILDING_API AProducingBuilding : public ABuildingBase
{
	GENERATED_BODY()
	
public:
	//How much resources needs to construct the building
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Settings", Meta=(ClampMin="0"))
	float ResourceBuildingCost = 0.f;

	//How much resources produces the building per second
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Settings", Meta = (ClampMin = "0"))
	float Productivity= 0.f;

	//Amount of produced resources
	UPROPERTY(BlueprintReadWrite, Category = "Settings", Meta = (ClampMin = "0"))
	float ResourcesProduced = 0.f;

	//Maximum building resources capacity
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Settings", Meta = (ClampMin = "0"))
	float ResourcesCapacity = 0.f;

	//How long building constructs in seconds
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Settings", Meta = (ClampMin = "0"))
	float ConstructionTime = 0.f;

public:
	//Start building construction and produce resources
	UFUNCTION(BlueprintImplementableEvent)
	void StartConstruction();
};
