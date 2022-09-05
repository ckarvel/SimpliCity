// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpliCityObjectManager.h"
#include "SimpliCityObjectBase.h"
#include "SimpliCityFunctionLibrary.h"
#include "GridManager.h"

ASimpliCityObjectManager::ASimpliCityObjectManager()
{ 
	PrimaryActorTick.bCanEverTick = false;
}

void ASimpliCityObjectManager::BeginPlay()
{
	Super::BeginPlay();
	AGridManager* gridMgr = USimpliCityFunctionLibrary::GetGridManager(this);
	InitializeArray(gridMgr->GetNumRows() * gridMgr->GetNumCols());
}

void ASimpliCityObjectManager::InitializeArray(int NumElements) {
	ObjectGrid.Init(nullptr,NumElements);
}

bool ASimpliCityObjectManager::DoesObjectExistHere(FVector Location) {
	AGridManager* gridMgr = USimpliCityFunctionLibrary::GetGridManager(this);
	int32 index = gridMgr->LocationToIndex(Location);
	check(gridMgr->IsIndexValid(index));
	ASimpliCityObjectBase* BuildObject = ObjectGrid[index];
	if(BuildObject != nullptr)
		return true;
	return false;
}

void ASimpliCityObjectManager::AddObjectToGrid(ASimpliCityObjectBase* AddedObject) {
	check(AddedObject != nullptr);
	FVector location = AddedObject->GetActorLocation();
	AGridManager* gridMgr = USimpliCityFunctionLibrary::GetGridManager(this);
	int32 index = gridMgr->LocationToIndex(location);
	check(gridMgr->IsIndexValid(index));
	ObjectGrid[index] = AddedObject;
}

void ASimpliCityObjectManager::ReplaceObjectInGrid(ASimpliCityObjectBase* OldObject, ASimpliCityObjectBase* NewObject) {
	check(OldObject != nullptr);
	if (DoesObjectExistHere(OldObject->GetActorLocation()) == false) {
		return; // object doesn't exist here so just ignore
	}
	RemoveObjectFromGrid(OldObject);
	AddObjectToGrid(NewObject);
}

void ASimpliCityObjectManager::RemoveObjectFromGrid(ASimpliCityObjectBase* RemovedObject) {
	check(RemovedObject != nullptr);
	FVector location = RemovedObject->GetActorLocation();
	RemoveObjectAtLocation(location);
}

void ASimpliCityObjectManager::RemoveObjectAtLocation(FVector Location) {
	AGridManager* gridMgr = USimpliCityFunctionLibrary::GetGridManager(this);
	int32 index = gridMgr->LocationToIndex(Location);
	check(gridMgr->IsIndexValid(index));
	if (ObjectGrid[index] != nullptr) {
		ObjectGrid[index]->Destroy();
	}
	ObjectGrid[index] = nullptr;
}

ASimpliCityObjectBase* ASimpliCityObjectManager::GetObjectAtLocation(FVector Location) {
	AGridManager* gridMgr = USimpliCityFunctionLibrary::GetGridManager(this);
	int32 index = gridMgr->LocationToIndex(Location);
	check(gridMgr->IsIndexValid(index));
	return ObjectGrid[index];
}

TArray<ASimpliCityObjectBase*> ASimpliCityObjectManager::GetNeighborsOfType(FVector Location,TEnumAsByte<ESimpliCityObjectType> ObjectType) {
	AGridManager* grdMgr = USimpliCityFunctionLibrary::GetGridManager(this);
	int32 currentIdx = grdMgr->LocationToIndex(Location);
	TArray<int32> neighborIdxs;
	grdMgr->GetValidNeighborIndexes(currentIdx,neighborIdxs);
	TArray<ASimpliCityObjectBase*> neighborsFound;
	for (auto idx : neighborIdxs) {
		if (ASimpliCityObjectBase* obj = ObjectGrid[idx]) {
			if (obj->Type() == ObjectType) {
				neighborsFound.Add(obj);
			}
		}
	}
	return neighborsFound;
}

TArray<ASimpliCityObjectBase*> ASimpliCityObjectManager::GetAllNeighbors(FVector Location) {
	AGridManager* grdMgr = USimpliCityFunctionLibrary::GetGridManager(this);
	int32 currentIdx = grdMgr->LocationToIndex(Location);
	TArray<int32> neighborIdxs;
	grdMgr->GetAllNeighborIndexes(currentIdx,neighborIdxs);
	TArray<ASimpliCityObjectBase*> allNeighbors;
	for (auto idx : neighborIdxs) {
		if (idx < 0)
			allNeighbors.Add(nullptr);
		else
			allNeighbors.Add(ObjectGrid[idx]);
	}
	return allNeighbors;
}
