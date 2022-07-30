// Fill out your copyright notice in the Description page of Project Settings.


#include "Builder/SimpliCityBuildObjectGrid.h"
#include "Builder/SimpliCityBuildObjectBase.h"
#include "Tools/SimpliCityFunctionLibrary.h"
#include "GridManager.h"

// Sets default values for this component's properties
USimpliCityBuildObjectGrid::USimpliCityBuildObjectGrid()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void USimpliCityBuildObjectGrid::BeginPlay()
{
	Super::BeginPlay();
	AGridManager* gridMgr = USimpliCityFunctionLibrary::GetGridManager(this);
	BuildObjectRefGrid.Init(nullptr,gridMgr->GetNumRows() * gridMgr->GetNumCols());
}

ASimpliCityBuildObjectBase* USimpliCityBuildObjectGrid::GetObjectAtLocation(FVector location) {
	int32 index = USimpliCityFunctionLibrary::GetGridManager(this)->LocationToIndex(location);
	return BuildObjectRefGrid[index];
}

bool USimpliCityBuildObjectGrid::DoesObjectExistHere(FVector location) {
  int32 index = USimpliCityFunctionLibrary::GetGridManager(this)->LocationToIndex(location);
  check(index >= 0 && index < BuildObjectRefGrid.Num());
	bool objectHere = false;
	if (ASimpliCityBuildObjectBase* BuildObject = BuildObjectRefGrid[index]) {
		//todo: i mean this shouldnt happen...
		objectHere = BuildObject->Type() != ESimpliCityBuildObjectEnum::BuildObject_None;
	}
  return objectHere;
}

void USimpliCityBuildObjectGrid::NotifySpawnedObject(ASimpliCityBuildObjectBase* SpawnedObject) {
	check(SpawnedObject != nullptr)
	FVector location = SpawnedObject->GetActorLocation();
	int32 index = USimpliCityFunctionLibrary::GetGridManager(this)->LocationToIndex(location);
	BuildObjectRefGrid[index] = SpawnedObject;
}

void USimpliCityBuildObjectGrid::NotifyDespawnedObject(ASimpliCityBuildObjectBase* DespawnedObject) {
	check(DespawnedObject != nullptr)
	FVector location = DespawnedObject->GetActorLocation();
	int32 index = USimpliCityFunctionLibrary::GetGridManager(this)->LocationToIndex(location);
	BuildObjectRefGrid[index] = nullptr;
}

void USimpliCityBuildObjectGrid::NotifyDespawnedObjectAtLocation(FVector location) {
	int32 index = USimpliCityFunctionLibrary::GetGridManager(this)->LocationToIndex(location);
	BuildObjectRefGrid[index] = nullptr;
}