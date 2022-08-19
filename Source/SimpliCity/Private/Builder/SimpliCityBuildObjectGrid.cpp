// Fill out your copyright notice in the Description page of Project Settings.


#include "Builder/SimpliCityBuildObjectGrid.h"
#include "Builder/SimpliCityBuildObjectBase.h"
#include "Tools/SimpliCityFunctionLibrary.h"
#include "GridManager.h"

// Sets default values for this component's properties
USimpliCityBuildObjectGrid::USimpliCityBuildObjectGrid()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void USimpliCityBuildObjectGrid::BeginPlay()
{
	Super::BeginPlay();
	AGridManager* gridMgr = USimpliCityFunctionLibrary::GetGridManager(this);
	InitializeArray(gridMgr->GetNumRows() * gridMgr->GetNumCols());
}

void USimpliCityBuildObjectGrid::InitializeArray(int NumElements) {
	BuildObjectRefGrid.Init(nullptr,NumElements);
}

TArray<ASimpliCityBuildObjectBase*> USimpliCityBuildObjectGrid::GetNeighborsOfType(FVector location,TEnumAsByte<ESimpliCityBuildObjectEnum> buildType) {
	AGridManager* grdMgr = USimpliCityFunctionLibrary::GetGridManager(this);
	int32 currentIdx = grdMgr->LocationToIndex(location);
	TArray<int32> neighborIdxs;
	grdMgr->GetNeighborIndexes(currentIdx,neighborIdxs);
	TArray<ASimpliCityBuildObjectBase*> neighborsFound;
	for (auto idx : neighborIdxs) {
		if (ASimpliCityBuildObjectBase* obj = BuildObjectRefGrid[idx]) {
			if (obj->Type() == buildType) {
				neighborsFound.Add(obj);
			}
		}
	}
	return neighborsFound;
}

TArray<ASimpliCityBuildObjectBase*> USimpliCityBuildObjectGrid::GetAllNeighbors_Unsafe(FVector location) {
	AGridManager* grdMgr = USimpliCityFunctionLibrary::GetGridManager(this);
	int32 currentIdx = grdMgr->LocationToIndex(location);
	TArray<int32> neighborIdxs;
	grdMgr->GetNeighborIndexes_Unsafe(currentIdx,neighborIdxs);
	TArray<ASimpliCityBuildObjectBase*> allNeighbors;
	for (auto idx : neighborIdxs) {
		if (idx < 0)
			allNeighbors.Add(nullptr);
		else
			allNeighbors.Add(BuildObjectRefGrid[idx]);
	}
	return allNeighbors;
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