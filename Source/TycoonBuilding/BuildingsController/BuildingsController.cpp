// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingsController.h"
#include "TycoonBuilding/Building/ProducingBuilding.h"
#include "Kismet/GameplayStatics.h"

//#include "DrawDebugHelpers.h"

// Sets default values
ABuildingsController::ABuildingsController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABuildingsController::BeginPlay()
{
	Super::BeginPlay();

	playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	BindToInput();
}

// Called every frame
void ABuildingsController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (chosingPlaceMode)
	{
		playerController->GetHitResultUnderCursorByChannel(traceChannel, true, cursorHitResult);
		constructedProdBuildings.Last()->SetActorLocation(cursorHitResult.Location);
	}
}

void ABuildingsController::GetBuildingsDefaultData(TArray<FBuildingMainData>& OutBuildingsDefaultData)
{
	FBuildingMainData data;
	//Get buildings default settings used for initial settings
	for (const auto &buildingType : BuildingTypesToConstruct)
	{
		if (buildingType)
		{
			AProducingBuilding* buildingCDO = Cast<AProducingBuilding>(buildingType->GetDefaultObject());
			data.BuildingName = buildingCDO->BuildingName;
			data.ReducedBuildingName = buildingCDO->ReducedBuildingName;
			data.ResourceBuildingCost = buildingCDO->ResourceBuildingCost;
			data.ConstructionTime = buildingCDO->ConstructionTime;
			data.Productivity = buildingCDO->Productivity;
			data.ResourcesCapacity = buildingCDO->ResourcesCapacity;
			data.BuildingType = buildingType;
			data.PreviewTexture = buildingCDO->PreviewTexture;
			OutBuildingsDefaultData.Add(data);
		}
	}
	//BuildingsDataCollectedDelegate.Broadcast(BuildingsDefaultData);
}

//Place and construct a building on scene
void ABuildingsController::ConstructBuilding(const TSubclassOf<AProducingBuilding>& BuildingType)
{
	check(BuildingType);
	AProducingBuilding* constructedBuilding = nullptr;
	constructedBuilding = GetWorld()->SpawnActor<AProducingBuilding>(BuildingType, FVector(0), FRotator::ZeroRotator);
	constructedProdBuildings.Add(constructedBuilding);

	traceChannel = UEngineTypes::ConvertToTraceType(ECC_Camera);
	chosingPlaceMode = true;
}

//Bind player input actions
void ABuildingsController::BindToInput()
{
	InputComponent = NewObject<UInputComponent>(this);
	InputComponent->RegisterComponent();
	if (InputComponent)
	{
		InputComponent->BindAction("LeftMouseBtn", IE_Released, this, &ABuildingsController::ConfirmPlaceToConstruct);
		InputComponent->BindAction("RightMouseBtn", IE_Released, this, &ABuildingsController::CancelConstructPlaceSearch);
		InputComponent->BindAxis("MouseWheelAxis", this, &ABuildingsController::RotateBuildingDuringPlaceSearch);

		EnableInput(GetWorld()->GetFirstPlayerController());
	}
}

//Place to construct selected - start building
void ABuildingsController::ConfirmPlaceToConstruct()
{
	if (chosingPlaceMode)
	{
		chosingPlaceMode = false;

		int lastIndex = constructedProdBuildings.Num() - 1;
		if (constructedProdBuildings.IsValidIndex(lastIndex))
		{
			auto constrBuilding = constructedProdBuildings[lastIndex];
			BuildingsConstructStartedDelegate.ExecuteIfBound(constrBuilding);
			constrBuilding->StartConstruction();
		}
	}
}

//Cancel building construct
void ABuildingsController::CancelConstructPlaceSearch()
{
	if (chosingPlaceMode)
	{
		chosingPlaceMode = false;
		int lastIndex = constructedProdBuildings.Num() - 1;
		if (constructedProdBuildings.IsValidIndex(lastIndex))
		{
			AProducingBuilding* buildingToDestroy = constructedProdBuildings[lastIndex];
			constructedProdBuildings.RemoveAt(lastIndex);
			buildingToDestroy->Destroy();
		}
	}
}

//Rotate building during planning
void ABuildingsController::RotateBuildingDuringPlaceSearch(float Value)
{
	if (Value != 0)
	{
		if (chosingPlaceMode)
		{
			int lastIndex = constructedProdBuildings.Num() - 1;
			if (constructedProdBuildings.IsValidIndex(lastIndex))
			{
				constructedProdBuildings[lastIndex]->AddActorWorldRotation(FRotator(0.f, BuildingRotationStep * Value, 0.f));
			}
		}
	}
}

float ABuildingsController::CollectResources() const
{
	float result = 0.f;
	for (const auto& building : constructedProdBuildings)
	{
		if (building != nullptr)
		{
			result += building->ResourcesProduced;
		}
	}
	return result;
}

//Spread construction cost between all producing buildings
void ABuildingsController::AllocteConstructionCost(float Cost)
{
	int32 numbOfBuildingsWithResources = GetNumberOfBuildingsWithResources();
	float allocatePerBuilding = Cost / numbOfBuildingsWithResources;

	AProducingBuilding* building;
	while (Cost > 0)
	{
		building = constructedProdBuildings[FMath::RandRange(0, numbOfBuildingsWithResources)];

		if (building->ResourcesProduced >= allocatePerBuilding)
		{
			if (building->ResourcesProduced <= Cost)
			{
				building->ResourcesProduced -= Cost;
				break;
			}

			building->ResourcesProduced -= allocatePerBuilding;
			Cost -= allocatePerBuilding;
		}
	}
}

int32 ABuildingsController::GetNumberOfBuildingsWithResources() const
{
	int32 result = 0;

	for (const auto building : constructedProdBuildings)
	{
		if (building->ResourcesProduced > 0)
		{
			result++;
		}
	}

	return result;
}