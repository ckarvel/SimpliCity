// Fill out your copyright notice in the Description page of Project Settings.

#include "SimpliCityBaseManager.h"

#include "GridManager.h"
#include "Kismet/GameplayStatics.h"
#include "SimpliCityFunctionLibrary.h"
#include "SimpliCityObjectManager.h"

using SCFL = USimpliCityFunctionLibrary;

//////////////////////////////////////////////////////////////////////////
ASimpliCityBaseManager::ASimpliCityBaseManager()
    : BuildEnabled(false)
    , CurrentlyBuilding(false)
    , ResourceType(ESimpliCityResourceType::None)
    , StartLocation(FVector(-1, -1, -1)) {
  PrimaryActorTick.bCanEverTick = true;
  // PrimaryActorTick.SetTickFunctionEnable(true);
  PrimaryActorTick.bStartWithTickEnabled = false;
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityBaseManager::BeginPlay() {
  Super::BeginPlay();
  GridManager = SCFL::GetGridManager(this);
  ObjectManager = SCFL::GetSimpliCityObjectManager(this);
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityBaseManager::StartBuilding() {
  CurrentlyBuilding = true;
  PrimaryActorTick.SetTickFunctionEnable(true);
}

//////////////////////////////////////////////////////////////////////////
bool ASimpliCityBaseManager::UpdateBuilding(FVector Location) {
  if (CurrentlyBuilding == false) {
    return false;
  }

  FVector currentLocation = GridManager->LocationToCenter(Location);

  // if just started, set start location
  if (GridManager->IsLocationValid(StartLocation) == false) {
    StartLocation = currentLocation;
  }

  if (LastLocation == currentLocation) {
    return false;
  }

  LastLocation = currentLocation;

  // return true if we received a new location
  return true;
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityBaseManager::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);
  // TRACE_LOG(LogSimpliCity, "ASimpliCityBuildingManager::Tick");
  APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
  if (PlayerController) {
    FHitResult HitResult;
    PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, HitResult);
    if (HitResult.bBlockingHit) {
      UpdateBuilding(HitResult.Location);
    }
  }
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityBaseManager::FinishBuilding() {
  // verify all temporary objects are valid if any exist
  BuildError = false;
  for (auto Object : Temporary_ObjectToLocation) {
    auto TempObject = Object.Value;
    if (TempObject->bIsValidPlacement == false) {
      BuildError = true;
      break;
    }
  }
  if (BuildError == false) {
    PayForSpawnedObjects();
    // disable build mode
    ASimpliCityBaseManager::CancelBuilding();
  }
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityBaseManager::CancelBuilding() {
  CurrentlyBuilding = false;
  StartLocation = FVector(-1, -1, -1);
  LastLocation = FVector(-1, -1, -1);
  PrimaryActorTick.SetTickFunctionEnable(false);
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityBaseManager::DestroyObject(ASimpliCityObjectBase* Object) {
  PermanentObjectList.Remove(Object);
  //SCFL::GetObjectManager(this)->RemoveObjectFromGrid(Object);
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityBaseManager::DestroyObjects(const TArray<ASimpliCityObjectBase*>& ObjectList) {
  for (auto Object : ObjectList) {
    if (Object == nullptr)
      continue;
    DestroyObject(Object);
  }
}

ASimpliCityObjectBase* ASimpliCityBaseManager::GetTemporaryObjectAtLocation(FVector Location) {
  ASimpliCityObjectBase* const* TempObject = Temporary_ObjectToLocation.Find(Location);
  if (TempObject != nullptr) {
    return *TempObject;
  }
  return nullptr;
}
