// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectManager.h"

#include "Kismet/GameplayStatics.h"

#include "GridManager.h"
#include "SimpliCityFunctionLibrary.h"
#include "SimpliCityObjectManager.h"
#include "SimpliCityObjectSelector.h"
#include "ObjectBase.h"
#include "SimpliCityGameState.h"

using SCFL = USimpliCityFunctionLibrary;

//////////////////////////////////////////////////////////////////////////
AObjectManager::AObjectManager()
{
  PrimaryActorTick.bCanEverTick = false;
}

//////////////////////////////////////////////////////////////////////////
void AObjectManager::BeginPlay()
{
  Super::BeginPlay();
  GridManager = SCFL::GetGridManager(this);
}

//////////////////////////////////////////////////////////////////////////
void AObjectManager::AddObject(AObjectBase* Object) {
  if (Object == nullptr) {
    TRACE_ERROR_PRINTF(LogSimpliCity, "ERROR!! Object == nullptr");
    return;
  }

  FVector CenterLocation = GridManager->LocationToCenter(Object->GetActorLocation());

  // add to master list
  if (LocationObjectMap.Contains(CenterLocation) == false) {
    LocationObjectMap.Add(CenterLocation);
  }
  TArray<AObjectBase*> LocObjects = *LocationObjectMap.Find(CenterLocation);
  LocObjects.Add(Object);
  LocationObjectMap.Emplace(CenterLocation, LocObjects);
  // add to type list (for easy finding)
  ESimpliCityObjectType Type = Object->GetObjectType();
  if (ObjectsPerTypeMap.Contains(Type) == false) {
    ObjectsPerTypeMap.Add(Type);
  }
  TArray<AObjectBase*> TypeObjects = *ObjectsPerTypeMap.Find(Type);
  TypeObjects.Add(Object);
  ObjectsPerTypeMap.Emplace(Type, TypeObjects);

  // broadcast new object
  OnNewObject.Broadcast(CenterLocation);
}

//////////////////////////////////////////////////////////////////////////
void AObjectManager::RemoveObject(AObjectBase* Object) {
  if (Object == nullptr) {
    TRACE_ERROR_PRINTF(LogSimpliCity, "ERROR!! Object == nullptr");
    return;
  }

  FVector CenterLocation = GridManager->LocationToCenter(Object->GetActorLocation());
  TArray<AObjectBase*> LocObjects = *LocationObjectMap.Find(CenterLocation);
  LocObjects.Remove(Object);
  LocationObjectMap.Emplace(CenterLocation, LocObjects);
  ESimpliCityObjectType Type = Object->GetObjectType();
  TArray<AObjectBase*> TypeObjects = *ObjectsPerTypeMap.Find(Type);
  TypeObjects.Remove(Object);
  ObjectsPerTypeMap.Emplace(Type, TypeObjects);

  OnRemovedObject.Broadcast(CenterLocation);
}

//////////////////////////////////////////////////////////////////////////
TArray<AObjectBase*> AObjectManager::GetObjects(FVector Location) {
  FVector CenterLocation = GridManager->LocationToCenter(Location);
  if (LocationObjectMap.Contains(CenterLocation)) {
    return *LocationObjectMap.Find(CenterLocation);
  }
  return TArray<AObjectBase*>();
}

//////////////////////////////////////////////////////////////////////////
TArray<AObjectBase*> AObjectManager::GetObjectsOfType(ESimpliCityObjectType Type) {
  if (ObjectsPerTypeMap.Contains(Type)) {
    return *ObjectsPerTypeMap.Find(Type);
  }
  return TArray<AObjectBase*>();
}
