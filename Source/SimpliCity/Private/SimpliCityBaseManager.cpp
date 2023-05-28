// Fill out your copyright notice in the Description page of Project Settings.

#include "SimpliCityBaseManager.h"

#include "GridManager.h"
#include "SimpliCityFunctionLibrary.h"
#include "SimpliCityObjectManager.h"
#include "SimpliObjectBase.h"

using SCFL = USimpliCityFunctionLibrary;

// Sets default values
ASimpliCityBaseManager::ASimpliCityBaseManager() : CurrentlyBuilding(false) {
  // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ASimpliCityBaseManager::BeginPlay() {
  Super::BeginPlay();
}

void ASimpliCityBaseManager::Update_Implementation() {
}

void ASimpliCityBaseManager::StartBuilding(FVector Location) {
  CurrentlyBuilding = true;
  StartLocation = SCFL::GetGridManager(this)->LocationToCenter(Location);
}

void ASimpliCityBaseManager::FinishBuilding() {
  CurrentlyBuilding = false;
  StartLocation = FVector();
}

void ASimpliCityBaseManager::CancelBuilding() {
  ASimpliCityBaseManager::FinishBuilding();
}

USimpliObjectBase* ASimpliCityBaseManager::CreateObject(TEnumAsByte<ESimpliCityObjectType> ObjectType,
                                                        const FVector Location) {
  USimpliObjectBase* Object = nullptr;
  switch (ObjectType) {
  case ESimpliCityObjectType::Road:
    Object = NewObject<USimpliRoadBase>(this, USimpliRoadBase::StaticClass());
    break;
  case ESimpliCityObjectType::Building:
    Object = NewObject<USimpliBuildingBase>(this, USimpliBuildingBase::StaticClass());
    break;
  case ESimpliCityObjectType::Zone:
    Object = NewObject<USimpliZoneBase>(this, USimpliZoneBase::StaticClass());
    break;

    if (Object) {
      Object->SpawnActor(Location);
    }
  }
  return Object;
}

ASimpliCityObjectBase* ASimpliCityBaseManager::PlaceObject(TSubclassOf<ASimpliCityObjectBase> ObjectClass,
                                                           const FVector Location, const FRotator Rotation) {
  ASimpliCityObjectBase* Object = nullptr;
  if (SCFL::GetObjectManager(this)->DoesObjectExistHere(Location)) {
    return Object;
  }
  SCFL::GetObjectManager(this)->AddObjectToGrid(Object);
  return Object;
}

ASimpliCityObjectBase* ASimpliCityBaseManager::PlaceTemporaryObject(TSubclassOf<ASimpliCityObjectBase> ObjectClass,
                                                                    const FVector Location, const FRotator Rotation) {
  ASimpliCityObjectBase* Object = PlaceObject(ObjectClass, Location, Rotation);
  if (Object) {
    Temporary_ObjectToLocation.Add(Location, Object);
  }
  return Object;
}

ASimpliCityObjectBase* ASimpliCityBaseManager::PlacePermanentObject(TSubclassOf<ASimpliCityObjectBase> ObjectClass,
                                                                    const FVector Location, const FRotator Rotation) {
  ASimpliCityObjectBase* Object = PlaceObject(ObjectClass, Location, Rotation);
  if (Object) {
    PermanentObjectList.Add(Object);
  }
  return Object;
}

void ASimpliCityBaseManager::DestroyObject(ASimpliCityObjectBase* Object) {
  PermanentObjectList.Remove(Object);
  SCFL::GetObjectManager(this)->RemoveObjectFromGrid(Object);
}

void ASimpliCityBaseManager::DestroyObjects(const TArray<ASimpliCityObjectBase*>& ObjectList) {
  for (auto Object : ObjectList) {
    if (Object == nullptr)
      continue;
    DestroyObject(Object);
  }
}

//// building manager has a spawned building already but just destroy and recreate
// bool ASimpliCityBuildingManager::PlacePermanentBuilding(ASimpliCityBuildingBase* Building) {
//   if (Building == nullptr)
//     return false;
//   if (SCFL::GetObjectManager(this)->DoesObjectExistHere(Building->GetActorLocation())) {
//     return false;
//   }
//   SCFL::GetObjectManager(this)->AddObjectToGrid(Building);
//   AddBuildingToList(Building->BuildingType, Building);
//   return true;
// }
//
// bool ASimpliCityZoneManager::PlacePermanentZoneBase(ASimpliCityZoneBase* ZoneBase) {
//   if (ZoneBase == nullptr)
//     return false;
//   if (SCFL::GetObjectManager(this)->DoesObjectExistHere(ZoneBase->GetActorLocation())) {
//     return false;
//   }
//   SCFL::GetObjectManager(this)->AddObjectToGrid(ZoneBase);
//   AddZoneBaseToList(ZoneBase->ZoneType, ZoneBase);
//   return true;
// }
//
// bool ASimpliCityRoadManager::PlacePermanentRoad(
//   TSubclassOf<ASimpliCityObjectBase> ObjectClass, const FVector Location, const FRotator Rotation) {
//   if (SCFL::GetObjectManager(this)->DoesObjectExistHere(Location)) {
//     return false;
//   }
//   ASimpliCityRoadBase* Road = SpawnRoad(RoadFixerComponent->GetDefaultRoadClass(), Location, Rotation, nullptr);
//   SCFL::GetObjectManager(this)->AddObjectToGrid(Road);
//   PermanentRoadList.Add(Road);
//   FixRoadAndNeighbors(Road);
//   return true;
// }
