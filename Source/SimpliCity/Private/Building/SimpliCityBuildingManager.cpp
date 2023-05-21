// Fill out your copyright notice in the Description page of Project Settings.

#include "Building/SimpliCityBuildingManager.h"

#include "Building/SimpliCityBuildingBase.h"
#include "SimpliCityFunctionLibrary.h"
#include "SimpliCityObjectManager.h"
#include "Utils/SimpliCityUtils.h"

using SCFL = USimpliCityFunctionLibrary;

ASimpliCityBuildingManager::ASimpliCityBuildingManager() {
  PrimaryActorTick.bCanEverTick = false;
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

void ASimpliCityBuildingManager::DestroyObjects(const TArray<ASimpliCityObjectBase*>& ObjectList) {
  for (auto Object : ObjectList) {
    if (Object == nullptr)
      continue;
    if (ASimpliCityBuildingBase* Building = Cast<ASimpliCityBuildingBase>(Object)) {
      RemoveBuildingFromList(Building);
      SCFL::GetObjectManager(this)->RemoveObjectFromGrid(Building);
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
