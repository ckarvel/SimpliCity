// Fill out your copyright notice in the Description page of Project Settings.

#include "Building/SimpliCityBuildingManager.h"

#include "Building/SimpliCityBuildingBase.h"
#include "GridManager.h"

#include "SimpliCityFunctionLibrary.h"
#include "SimpliCityObjectManager.h"
#include "Utils/SimpliCityUtils.h"

using SCFL = USimpliCityFunctionLibrary;

ASimpliCityBuildingManager::ASimpliCityBuildingManager()
    : ActiveObject(nullptr) {
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityBuildingManager::Enable(UTexture2D* NewIcon) {
  // if desired object type is new, change our active object (if exists)
  if (NewIcon && BuildIcon != NewIcon) {
    BuildEnabled = true;
    BuildIcon = NewIcon;
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
  BuildIcon = nullptr;
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityBuildingManager::StartBuilding() {
  ASimpliCityBaseManager::StartBuilding();

  // spawn a new building, destroying any existing
  if (ActiveObject != nullptr) {
    ActiveObject->Destroy();
    ActiveObject = nullptr;
  }
  ActiveObject = SpawnObjectOfType(DefaultBlueprintClass, FVector(0, 0, -1000), FRotator(0, 0, 0), BuildIcon);

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
    ActiveObject->SetActorLocation(LastLocation);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityBuildingManager::FinishBuilding() {
  ASimpliCityBaseManager::FinishBuilding();
  PermanentObjectList.Add(ActiveObject);
  ActiveObject = nullptr;
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityBuildingManager::CancelBuilding() {
  ASimpliCityBaseManager::CancelBuilding();
  if (ActiveObject != nullptr) {
    ActiveObject->Destroy();
    ActiveObject = nullptr;
  }
}

bool ASimpliCityBuildingManager::PlacePermanentBuilding(ASimpliCityBuildingBase* Building) {
  if (Building == nullptr)
    return false;
  if (SCFL::GetObjectManager(this)->DoesObjectExistHere(Building->GetActorLocation())) {
    return false;
  }
  SCFL::GetObjectManager(this)->AddObjectToGrid(Building);
  AddBuildingToList(Building->BuildingType, Building);
  return true;
}

ASimpliCityObjectBase* ASimpliCityBuildingManager::PlacePermanentObject(TSubclassOf<ASimpliCityObjectBase> ObjectClass,
                                                                        const FVector Location,
                                                                        const FRotator Rotation) {
  ASimpliCityObjectBase* Object = ASimpliCityBaseManager::PlacePermanentObject(ObjectClass, Location, Rotation);
  if (Object == nullptr) {
    return Object;
  }
  ASimpliCityBuildingBase* Building = Cast<ASimpliCityBuildingBase>(Object);
  AddBuildingToList(Building->BuildingType, Building);
  return Object;
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
  ESimpliCityBuildingType Type = Building->BuildingType;
  if (BuildingListPerType.Contains(Type)) {
    TArray<ASimpliCityBuildingBase*> BuildingList = BuildingListPerType[Type];
    if (BuildingList.Contains(Building)) {
      BuildingList.Remove(Building);
      BuildingListPerType[Type] = BuildingList;
    }
  }
}
