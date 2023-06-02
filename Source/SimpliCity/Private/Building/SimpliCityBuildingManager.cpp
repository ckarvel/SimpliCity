// Fill out your copyright notice in the Description page of Project Settings.

#include "Building/SimpliCityBuildingManager.h"

#include "Building/SimpliCityBuildingBase.h"
#include "GridManager.h"
#include "Kismet/GameplayStatics.h"
#include "SimpliCityFunctionLibrary.h"
#include "SimpliCityObjectManager.h"
#include "Utils/SimpliCityUtils.h"

using SCFL = USimpliCityFunctionLibrary;

ASimpliCityBuildingManager::ASimpliCityBuildingManager() : ActiveObject(nullptr) {
  PrimaryActorTick.bCanEverTick = true;
  // PrimaryActorTick.SetTickFunctionEnable(true);
  PrimaryActorTick.bStartWithTickEnabled = false;
}

void ASimpliCityBuildingManager::Enable(UTexture2D* NewIcon) {
  // if desired object type is new, change our active object (if exists)
  if (NewIcon && BuildIcon != NewIcon) {
    BuildEnabled = true;
    BuildIcon = NewIcon;
    PrimaryActorTick.SetTickFunctionEnable(true);
    StartBuilding(FVector(0, 0, -1000));
  }
}

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
void ASimpliCityBuildingManager::StartBuilding(FVector Location) {
  ASimpliCityBaseManager::StartBuilding(Location);
  // spawn building from class
  if (ActiveObject != nullptr) {
    ActiveObject->Destroy();
    ActiveObject = nullptr;
  }

  ActiveObject = SpawnObjectOfType(DefaultBlueprintClass, Location, FRotator(0, 0, 0), BuildIcon);
#if WITH_EDITOR
  ActiveObject->SetFolderPath("Buildings");
#endif

  PrimaryActorTick.SetTickFunctionEnable(true);
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityBuildingManager::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);
  // TRACE_LOG(LogSimpliCity, "ASimpliCityBuildingManager::Tick");
  APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
  if (PlayerController) {
    FHitResult HitResult;
    PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, HitResult);
    if (HitResult.bBlockingHit) {
      Update(HitResult.Location);
    }
  }
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityBuildingManager::Update(FVector Location) {
  if (CurrentlyBuilding == false) {
    return;
  }
  AGridManager* gridMgr = SCFL::GetGridManager(this);
  FVector currentLocation = gridMgr->LocationToCenter(Location);
  if (LastLocation == currentLocation)
    return; // mouse on same tile as before, so exit
  LastLocation = currentLocation;

  // todo: if valid (empty & on grid) set location to grid center
  // if invalid (not empty or not grid) set location to mouse cursor
  if (ActiveObject) {
    ActiveObject->SetActorLocation(currentLocation);
  } // otherwise??
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityBuildingManager::FinishBuilding() {
  ASimpliCityBaseManager::FinishBuilding();
  PermanentObjectList.Add(ActiveObject);
  ActiveObject = nullptr;
  PrimaryActorTick.SetTickFunctionEnable(false);
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityBuildingManager::CancelBuilding() {
  ASimpliCityBaseManager::CancelBuilding();
  if (ActiveObject != nullptr) {
    ActiveObject->Destroy();
    ActiveObject = nullptr;
  }
  PrimaryActorTick.SetTickFunctionEnable(false);
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
