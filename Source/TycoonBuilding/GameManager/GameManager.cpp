// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"
#include "TycoonBuilding/BuildingsController/BuildingsController.h"
#include "TycoonBuilding/Building/ProducingBuilding.h"

// Sets default values
AGameManager::AGameManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AGameManager::BeginPlay()
{
	Super::BeginPlay();
	
	check(BuildingsController);
	GetWorld()->GetTimerManager().SetTimer(UpdateBudgetTimeHandler, this, &AGameManager::CollectResources, 0.5f, true);
	BuildingsController->BuildingsConstructStartedDelegate.BindUObject(this, &AGameManager::SpendResourcesToBuildingConstruction);
}

// Called every frame
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGameManager::CollectResources()
{
	if (BuildingsController)
	{
		ResourcesBudget = StartResourcesBudget + BuildingsController->CollectResources();
	}

	BudgetUpdated(ResourcesBudget);
}

void AGameManager::SpendResourcesToBuildingConstruction(AProducingBuilding* ConstructingBuilding)
{
	float constructionCost = ConstructingBuilding->ResourceBuildingCost;
	if (StartResourcesBudget > 0)
	{
		//+float remains = abs(StartResourcesBudget - constructionCost);
		if (StartResourcesBudget >= constructionCost)
		{
			StartResourcesBudget -= constructionCost;
			return;
		}
		else
		{
			constructionCost -= StartResourcesBudget;
			StartResourcesBudget = 0;
		}
	}

	if (StartResourcesBudget == 0)
	{
		BuildingsController->AllocteConstructionCost(constructionCost);
	}
}