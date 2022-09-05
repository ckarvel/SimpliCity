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

ASimpliCityBuildingBase* ASimpliCityBuildingManager::GetAnyBuildingWithSupply(ESimpliCityBuildingEnum Type) {
	TArray<ASimpliCityBuildingBase*> BuildingsOfType = BuildingListPerType.FindOrAdd(Type);
	for (auto Building : BuildingsOfType) {
		//if(Building->)
	}
	return nullptr;
}

void ASimpliCityBuildingManager::AddBuildingToList(ESimpliCityBuildingEnum Type,ASimpliCityBuildingBase* Building) {}

void ASimpliCityBuildingManager::RemoveBuildingFromList(ESimpliCityBuildingEnum Type,ASimpliCityBuildingBase* Building) {}
