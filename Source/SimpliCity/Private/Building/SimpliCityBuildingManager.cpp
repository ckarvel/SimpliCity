// Fill out your copyright notice in the Description page of Project Settings.

#include "Building/SimpliCityBuildingManager.h"

#include "Building/SimpliCityBuildingBase.h"

#include "GridManager.h"
#include "SimpliCityObjectManager.h"
#include "Utils/SimpliCityUtils.h"

ASimpliCityBuildingManager::ASimpliCityBuildingManager()
    : ActiveObject(nullptr) {
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityBuildingManager::Enable(ESimpliCityResourceType _TypeId) {
  // if desired object type is new, change our active object (if exists)
  if (ResourceType != _TypeId) {
    BuildEnabled = true;
    ResourceType = _TypeId;
    StartBuilding();
  }
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityBuildingManager::Disable() {
  if (BuildEnabled == false)
    return; // already disabled

  if (CurrentlyBuilding) {
    CancelBuilding();
  }

  BuildEnabled = false;
  ResourceType = ESimpliCityResourceType::None;
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityBuildingManager::StartBuilding() {
  ASimpliCityBaseManager::StartBuilding();

  // spawn a new building, destroying any existing
  if (ActiveObject != nullptr) {
    ActiveObject->Destroy();
    ActiveObject = nullptr;
  }
  ActiveObject = SpawnObjectOfType(DefaultBlueprintClass, FVector(0, 0, -1000), FRotator(0, 0, 0), ResourceType);

#if WITH_EDITOR
  ActiveObject->SetFolderPath("Buildings");
#endif
}

//////////////////////////////////////////////////////////////////////////
bool ASimpliCityBuildingManager::UpdateBuilding(FVector Location) {
  if (ASimpliCityBaseManager::UpdateBuilding(Location) == false) {
    return false;
  }

  if (ActiveObject) {
    ActiveObject->SetNewLocation(LastLocation);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityBuildingManager::FinishBuilding() {
  if (ActiveObject == nullptr) { // how?! this has happened but... how
    TRACE_ERROR_PRINTF(LogSimpliCity, "ERROR!! BuildingManager::FinishBuilding() ActiveObject == nullptr");
    ASimpliCityBaseManager::FinishBuilding();
    return;
  }
  // HACK- Add active object to temp list so base manager
  // can detect if build error
  Temporary_ObjectToLocation.Empty();
  Temporary_ObjectToLocation.Add(FVector(), ActiveObject);
  ASimpliCityBaseManager::FinishBuilding();
  if (BuildError) {
    return;
  }
  PermanentObjectList.Add(ActiveObject);
  ObjectManager->AddObjectToGrid(ActiveObject);
  ActiveObject->OnObjectPlaced();
  ActiveObject = nullptr;

  // sabotage so we can re-enable
  ESimpliCityResourceType CurrentResourceType = ResourceType;
  ResourceType = ESimpliCityResourceType::None;
  // start over with a new building (same resource)
  Enable(CurrentResourceType);
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityBuildingManager::CancelBuilding() {
  ASimpliCityBaseManager::CancelBuilding();
  if (ActiveObject != nullptr) {
    ActiveObject->Destroy();
    ActiveObject = nullptr;
  }
}

void ASimpliCityBuildingManager::DestroyObjects(const TArray<ASimpliCityObjectBase*>& ObjectList) {
  for (auto Object : ObjectList) {
    if (Object == nullptr)
      continue;
    if (ASimpliCityBuildingBase* Building = Cast<ASimpliCityBuildingBase>(Object)) {
      RemoveBuildingFromList(Building);
      ASimpliCityBaseManager::DestroyObject(Object);
    }
  }
}

TArray<ASimpliCityBuildingBase*> ASimpliCityBuildingManager::GetAllBuildingsOfType(ESimpliCityBuildingType Type) {
  TArray<ASimpliCityBuildingBase*> BuildingsOfType = BuildingListPerType.FindOrAdd(Type);
  return BuildingsOfType;
}

void ASimpliCityBuildingManager::AddBuildingToList(ESimpliCityBuildingType Type, ASimpliCityBuildingBase* Building) {
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
  ESimpliCityBuildingType Type = Building->GetBuildingType();
  if (BuildingListPerType.Contains(Type)) {
    TArray<ASimpliCityBuildingBase*> BuildingList = BuildingListPerType[Type];
    if (BuildingList.Contains(Building)) {
      BuildingList.Remove(Building);
      BuildingListPerType[Type] = BuildingList;
    }
  }
}
