 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameManager.generated.h"

class ABuildingsController;
class AProducingBuilding;

/**
 * Game manager
 * API between BackEnd (C++ core) and FrontEnd
 */
UCLASS()
class TYCOONBUILDING_API AGameManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	ABuildingsController* BuildingsController;

	//Start resources budget
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Settings", Meta = (ClampMin = "0"))
	float StartResourcesBudget = 0.f;

	//All resources budget
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Settings", Meta = (ClampMin = "0"))
	float ResourcesBudget = 0.f;

public:
	//Start building construction and produce resources
	UFUNCTION(BlueprintImplementableEvent)
	void BudgetUpdated(float Resources);

private:
	void CollectResources();
	void SpendResourcesToBuildingConstruction(AProducingBuilding* ConstructingBuilding);
private:
	FTimerHandle UpdateBudgetTimeHandler;
};
