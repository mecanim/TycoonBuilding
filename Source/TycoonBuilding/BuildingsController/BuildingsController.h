// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TycoonBuilding/Structs/BuildingMainData.h"
#include "BuildingsController.generated.h"

class AProducingBuilding;

UCLASS()
class TYCOONBUILDING_API ABuildingsController : public AActor
{
	GENERATED_BODY()

	DECLARE_DELEGATE_OneParam(FBuildingsConstructStarted, AProducingBuilding*);
	
public:	
	// Sets default values for this actor's properties
	ABuildingsController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	//Add here all building types that will be showed in menu
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Settings")
	TArray<TSubclassOf<AProducingBuilding>> BuildingTypesToConstruct;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	float BuildingRotationStep = 90.f;

	FBuildingsConstructStarted BuildingsConstructStartedDelegate;

public:
	//Returns main data from BuildingTypesToConstruct for buildings menu
	UFUNCTION(BlueprintCallable, Category = "Buildings Controller")
	void ConstructBuilding(const TSubclassOf<AProducingBuilding>& BuildingType);

	//Get buildings classed default data. Called from UMG to place and construct a building on scene
	UFUNCTION(BlueprintCallable, Category = "Buildings Controller")
	void GetBuildingsDefaultData(TArray<FBuildingMainData>& OutBuildingsDefaultData);

	//Get resources from all producing buildings
	float CollectResources() const;

	//Spread construction cost between all producing buildings
	void AllocteConstructionCost(float Cost);

private:
	//Bind player input actions
	void BindToInput();
	
	//Place to construct selected - start building
	void ConfirmPlaceToConstruct();

	//Cancel building construct
	void CancelConstructPlaceSearch();

	//Rotate building during planning
	void RotateBuildingDuringPlaceSearch(float Value);

	//How many buildings have resources > 0
	int32 GetNumberOfBuildingsWithResources() const;

private:
	APlayerController* playerController;
	ETraceTypeQuery traceChannel;
	FHitResult cursorHitResult;

	//Producing buildings
	TArray<AProducingBuilding*> constructedProdBuildings;

	bool chosingPlaceMode = false;
};
