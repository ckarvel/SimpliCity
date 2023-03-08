// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Building/SimpliCityBuildingType.h"
#include "SimpliCityBuildingManager.generated.h"

UCLASS()
class SIMPLICITY_API ASimpliCityBuildingManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ASimpliCityBuildingManager();

protected:

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="SimpliCityBuildingManager")
	bool IsCurrentlyBuilding;

public:
	UFUNCTION(BlueprintCallable, Category = "SimpliCityBuildingManager")
	bool PlacePermanentBuilding(ASimpliCityBuildingBase* Building);

	UFUNCTION(BlueprintCallable, Category = "SimpliCityBuildingManager")
	void DestroyObjects(const TArray<ASimpliCityObjectBase*>& ObjectList);

	UFUNCTION(BlueprintCallable,Category="SimpliCityBuildingManager")
	TArray<class ASimpliCityBuildingBase*> GetAllBuildingsOfType(ESimpliCityBuildingType Type);
	void AddBuildingToList(ESimpliCityBuildingType Type, ASimpliCityBuildingBase* Building);
	void RemoveBuildingFromList(ASimpliCityBuildingBase* Building);

	TMap<ESimpliCityBuildingType,TArray<ASimpliCityBuildingBase*>> BuildingListPerType;
};
