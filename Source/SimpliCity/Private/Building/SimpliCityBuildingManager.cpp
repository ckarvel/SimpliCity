// Fill out your copyright notice in the Description page of Project Settings.


#include "Building/SimpliCityBuildingManager.h"
#include "Building/SimpliCityBuildingBase.h"
#include "SimpliCityFunctionLibrary.h"
#include "Utils/SimpliCityUtils.h"


// Sets default values
ASimpliCityBuildingManager::ASimpliCityBuildingManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ASimpliCityBuildingManager::BeginPlay()
{
	Super::BeginPlay();
}

TArray<ASimpliCityBuildingBase*> ASimpliCityBuildingManager::GetAllBuildingsOfType(ESimpliCityBuildingType Type) {
	TArray<ASimpliCityBuildingBase*> BuildingsOfType = BuildingListPerType.FindOrAdd(Type);
	return BuildingsOfType;
}

void ASimpliCityBuildingManager::AddBuildingToList(ESimpliCityBuildingType Type,ASimpliCityBuildingBase* Building) {
	if (BuildingListPerType.Contains(Type)) {
		TArray<ASimpliCityBuildingBase*> BuildingList = BuildingListPerType[Type];
		if (BuildingList.Contains(Building) == false) {
			BuildingList.Add(Building);
			BuildingListPerType[Type] = BuildingList;
		}
	}
}

void ASimpliCityBuildingManager::RemoveBuildingFromList(ASimpliCityBuildingBase* Building) {
	if (Building == nullptr)
		return;
	ESimpliCityBuildingType Type = Building->BuildingType;
	if (BuildingListPerType.Contains(Type)) {
		TArray<ASimpliCityBuildingBase*> BuildingList = BuildingListPerType[Type];
		if (BuildingList.Contains(Building)) {
			BuildingList.Remove(Building);
			BuildingListPerType[Type] = BuildingList;
		}
	}
}
