// Fill out your copyright notice in the Description page of Project Settings.

#include "SimpliCityBaseManager.h"

#include "GridManager.h"
#include "SimpliCityFunctionLibrary.h"
#include "SimpliCityObjectManager.h"
#include "SimpliObjectBase.h"

using SCFL = USimpliCityFunctionLibrary;

// Sets default values
ASimpliCityBaseManager::ASimpliCityBaseManager() : BuildEnabled(false), CurrentlyBuilding(false) {
  PrimaryActorTick.bCanEverTick = true;
  // PrimaryActorTick.SetTickFunctionEnable(true);
  PrimaryActorTick.bStartWithTickEnabled = false;
}

void ASimpliCityBaseManager::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);
}

// Called when the game starts or when spawned
void ASimpliCityBaseManager::BeginPlay() {
  Super::BeginPlay();
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
