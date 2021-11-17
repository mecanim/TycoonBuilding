// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingBase.generated.h"

UCLASS(Abstract)
class TYCOONBUILDING_API ABuildingBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildingBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Settings")
	FString BuildingName;

	//Abbreaviated name to fit into ui. If empty BuildingName is using
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Settings")
	FString ReducedBuildingName;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Settings")
	UTexture2D* PreviewTexture;
};
