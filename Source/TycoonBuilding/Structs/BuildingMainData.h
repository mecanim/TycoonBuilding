#pragma once

#include "CoreMinimal.h"
#include "TycoonBuilding/Building/ProducingBuilding.h"
#include "BuildingMainData.generated.h"

/**
 * Data describing buildings
 */

USTRUCT(BlueprintType)
struct FBuildingMainData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString BuildingName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ReducedBuildingName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ResourceBuildingCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Productivity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ResourcesCapacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ConstructionTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AProducingBuilding> BuildingType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* PreviewTexture;
};
