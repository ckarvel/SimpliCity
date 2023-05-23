// Fill out your copyright notice in the Description page of Project Settings.

#include "SimpliCityObjectManager.h"

#include "GridManager.h"
#include "SimpliCityFunctionLibrary.h"
#include "SimpliCityObjectBase.h"

ASimpliCityObjectManager::ASimpliCityObjectManager() : bInitialized(false) {
  PrimaryActorTick.bCanEverTick = false;
}

void ASimpliCityObjectManager::BeginPlay() {
  Super::BeginPlay();
  AGridManager* gridMgr = USimpliCityFunctionLibrary::GetGridManager(this);
  InitializeArray(gridMgr->GetNumRows() * gridMgr->GetNumCols());
}

void ASimpliCityObjectManager::InitializeArray(int NumElements) {
  if (bInitialized == false) {
    ObjectGrid.Init(nullptr, NumElements);
    bInitialized = true;
  }
}

bool ASimpliCityObjectManager::DoesObjectExistHere(FVector Location) {
  AGridManager* gridMgr = USimpliCityFunctionLibrary::GetGridManager(this);
  int32 index = gridMgr->LocationToIndex(Location);
  if (gridMgr->IsIndexValid(index)) {
    ASimpliCityObjectBase* BuildObject = ObjectGrid[index];
    if (BuildObject != nullptr)
      return true;
  }
  return false;
}

void ASimpliCityObjectManager::AddObjectToGrid(ASimpliCityObjectBase* AddedObject) {
  if (AddedObject == nullptr) {
    TRACE_ERROR_PRINTF(LogSimpliCity, "ERROR!! AddedObject == nullptr");
    return;
  }
  FVector location = AddedObject->GetActorLocation();
  AGridManager* gridMgr = USimpliCityFunctionLibrary::GetGridManager(this);
  int32 index = gridMgr->LocationToIndex(location);
  if (!gridMgr->IsIndexValid(index)) {
    TRACE_ERROR_PRINTF(LogSimpliCity, "ERROR!! !gridMgr->IsIndexValid(%d)", index);
    return;
  }
  ObjectGrid[index] = AddedObject;

  // Broadcast when spawning a new object, not replacing
  if (!IsReplacing)
    OnSpawnedObject.Broadcast(location);
}

void ASimpliCityObjectManager::ReplaceObjectInGrid(ASimpliCityObjectBase* OldObject, ASimpliCityObjectBase* NewObject) {
  if (OldObject == nullptr) {
    TRACE_ERROR_PRINTF(LogSimpliCity, "ERROR!! OldObject == nullptr");
    return;
  }
  if (DoesObjectExistHere(OldObject->GetActorLocation()) == false) {
    return; // object doesn't exist here so just ignore
  }
  IsReplacing = true;
  RemoveObjectFromGrid(OldObject);
  AddObjectToGrid(NewObject);
  IsReplacing = false;
}

void ASimpliCityObjectManager::RemoveObjectFromGrid(ASimpliCityObjectBase* RemovedObject) {
  if (RemovedObject == nullptr) {
    TRACE_ERROR_PRINTF(LogSimpliCity, "ERROR!! RemovedObject == nullptr");
    return;
  }
  FVector location = RemovedObject->GetActorLocation();
  RemoveObjectAtLocation(location);
}

void ASimpliCityObjectManager::RemoveObjectAtLocation(FVector Location) {
  AGridManager* gridMgr = USimpliCityFunctionLibrary::GetGridManager(this);
  int32 index = gridMgr->LocationToIndex(Location);
  if (!gridMgr->IsIndexValid(index)) {
    TRACE_ERROR_PRINTF(LogSimpliCity, "ERROR!! !gridMgr->IsIndexValid(%d)", index);
    return;
  }
  if (ObjectGrid[index] != nullptr) {
    ObjectGrid[index]->Destroy();
  }
  ObjectGrid[index] = nullptr;

  // Broadcast when truly destroying & won't replace
  if (!IsReplacing)
    OnDestroyedObject.Broadcast(Location);
}

ASimpliCityObjectBase* ASimpliCityObjectManager::GetObjectAtLocation(FVector Location) {
  AGridManager* gridMgr = USimpliCityFunctionLibrary::GetGridManager(this);
  int32 index = gridMgr->LocationToIndex(Location);
  if (!gridMgr->IsIndexValid(index)) {
    TRACE_ERROR_PRINTF(LogSimpliCity, "ERROR!! !gridMgr->IsIndexValid(%d)", index);
    return nullptr;
  }
  return ObjectGrid[index];
}

TArray<ASimpliCityObjectBase*>
ASimpliCityObjectManager::GetNeighborsOfType(FVector Location, TEnumAsByte<ESimpliCityObjectType> ObjectType) {
  AGridManager* grdMgr = USimpliCityFunctionLibrary::GetGridManager(this);
  int32 currentIdx = grdMgr->LocationToIndex(Location);
  TArray<int32> neighborIdxs;
  grdMgr->GetValidNeighborIndexes(currentIdx, neighborIdxs);
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
  grdMgr->GetAllNeighborIndexes(currentIdx, neighborIdxs);
  TArray<ASimpliCityObjectBase*> allNeighbors;
  for (auto idx : neighborIdxs) {
    if (idx < 0)
      allNeighbors.Add(nullptr);
    else
      allNeighbors.Add(ObjectGrid[idx]);
  }
  return allNeighbors;
}

bool ASimpliCityObjectManager::IsNearRoad(FVector Location) {
  TArray<ASimpliCityObjectBase*> Neighbors = GetNeighborsOfType(Location, ESimpliCityObjectType::Road);
  if (Neighbors.Num() > 0)
    return true;
  return false;
}
