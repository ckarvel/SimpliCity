// Fill out your copyright notice in the Description page of Project Settings.

#include "Road/SimpliCityRoadManager.h"

#include "GameFramework/PlayerController.h"
#include "GridManager.h"
#include "Kismet/GameplayStatics.h"
#include "MarkerManager.h"
#include "Road/SimpliCityRoadBase.h"
#include "Road/SimpliCityRoadFixerComponent.h"
#include "SimpliCityFunctionLibrary.h"
#include "SimpliCityObjectBase.h"
#include "SimpliCityObjectManager.h"

using SCFL = USimpliCityFunctionLibrary;

//////////////////////////////////////////////////////////////////////////
ASimpliCityRoadManager::ASimpliCityRoadManager() {
  PrimaryActorTick.bCanEverTick = false;
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityRoadManager::Update_Implementation() {
  APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
  if (PlayerController) {
    FHitResult HitResult;
    PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, HitResult);
    if (HitResult.bBlockingHit) {
      UpdatePath(HitResult.Location);
    }
  }
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityRoadManager::StartBuilding(FVector Location) {
  ASimpliCityBaseManager::StartBuilding(Location);
  oldPath.Empty();
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityRoadManager::FinishBuilding() {
  ASimpliCityBaseManager::FinishBuilding();
  oldPath.Empty();
  ConvertAllTemporaryToPermanent();
  TArray<UObject*> NewRoads;
  NewRoads.Append(PermanentObjectList);
  AgentMarkerGraph->UpdateGraph(NewRoads);
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityRoadManager::CancelBuilding() {
  ASimpliCityBaseManager::CancelBuilding();
  oldPath.Empty();
  DestroyAllTemporaryRoads();
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityRoadManager::DestroyObjects(const TArray<ASimpliCityObjectBase*>& ObjectList) {
  for (auto Object : ObjectList) {
    if (Object == nullptr)
      continue;
    if (ASimpliCityRoadBase* Road = Cast<ASimpliCityRoadBase>(Object)) {
      TArray<ASimpliCityObjectBase*> neighborRoads =
          SCFL::GetObjectManager(this)->GetNeighborsOfType(Road->GetActorLocation(), ESimpliCityObjectType::Road);
      ASimpliCityBaseManager::DestroyObject(Object);
      for (auto nroad : neighborRoads) {
        // if neighbor is not in the removal list, fix it
        if (ObjectList.Contains(nroad) == false)
          FixRoad(Cast<ASimpliCityRoadBase>(nroad));
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////
// BUILD
//////////////////////////////////////////////////////////////////////////
void ASimpliCityRoadManager::StartPlacingRoad(FVector Location) {
  oldPath.Empty();
  StartLocation = SCFL::GetGridManager(this)->LocationToCenter(Location);
  CurrentlyBuilding = true;
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityRoadManager::UpdatePath(FVector Location) {
  if (CurrentlyBuilding == false) {
    return;
  }
  AGridManager* gridMgr = SCFL::GetGridManager(this);
  FVector currentLocation = gridMgr->LocationToCenter(Location);
  if (LastLocation == currentLocation)
    return; // mouse on same tile as before, so exit
  LastLocation = currentLocation;
  // calculate new path between start & given location
  TSet<FVector> newPath = TSet<FVector>(SCFL::GetPathBetween(gridMgr, StartLocation, currentLocation));
  TSet<FVector> removeRoads = oldPath.Difference(newPath);
  TArray<FVector> removeRoadsArr = removeRoads.Array();
  ASimpliCityObjectManager* objectMgr = SCFL::GetObjectManager(this);
  for (auto const& roadLoc : removeRoadsArr) {
    // if this location is not a temporary road, then we don't want to remove this road.
    if (Temporary_ObjectToLocation.Contains(roadLoc) == false) {
      removeRoads.Remove(roadLoc);
      // TRACE_SCREENMSG_PRINTF("Don't remove: Permanent object exists here!");
    }
  }
  DestroyTemporaryRoadsAtLocations(removeRoads.Array());
  TSet<FVector> createRoads = newPath.Difference(oldPath);
  TArray<FVector> createRoadsArr = createRoads.Array();
  // temporary roads to create
  for (auto const& roadLoc : createRoadsArr) {
    if (objectMgr->DoesObjectExistHere(roadLoc) == true) {
      createRoads.Remove(roadLoc);
      // TRACE_SCREENMSG_PRINTF("Don't create: Permanent object exists here!");
    }
  }
  CreateTemporaryRoadsAtLocations(createRoads.Array());
  oldPath = newPath;
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityRoadManager::FinishBuildingPath() {
  oldPath.Empty();
  StartLocation = FVector();
  CurrentlyBuilding = false;
  ConvertAllTemporaryToPermanent();
  TArray<UObject*> NewRoads;
  NewRoads.Append(PermanentObjectList);
  AgentMarkerGraph->UpdateGraph(NewRoads);
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityRoadManager::CancelBuildingPath() {
  oldPath.Empty();
  StartLocation = FVector();
  CurrentlyBuilding = false;
  DestroyAllTemporaryRoads();
}

//////////////////////////////////////////////////////////////////////////
ASimpliCityRoadBase* ASimpliCityRoadManager::SpawnRoad_Implementation(TSubclassOf<ASimpliCityRoadBase> RoadClass,
                                                                      const FVector Location, const FRotator Rotation,
                                                                      ASimpliCityObjectBase* RoadBeingReplaced) {
  ASimpliCityRoadBase* Road = GetWorld()->SpawnActor<ASimpliCityRoadBase>(RoadClass, Location, Rotation);
  return Road;
}

//////////////////////////////////////////////////////////////////////////
bool ASimpliCityRoadManager::PlacePermanentRoad(const FVector Location, const FRotator Rotation) {
  if (SCFL::GetObjectManager(this)->DoesObjectExistHere(Location)) {
    return false;
  }
  ASimpliCityRoadBase* Road = SpawnRoad(RoadFixerComponent->GetDefaultRoadClass(), Location, Rotation, nullptr);
  SCFL::GetObjectManager(this)->AddObjectToGrid(Road);
  PermanentObjectList.Add(Road);
  FixRoadAndNeighbors(Road);
  return true;
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityRoadManager::CreateTemporaryRoadsAtLocations(const TArray<FVector>& Locations) {
  for (auto Location : Locations) {
    ASimpliCityRoadBase* TempRoad = SpawnRoad(RoadFixerComponent->GetDefaultRoadClass(), Location, FRotator(), nullptr);
    // PlaceTemporaryObject(RoadFixerComponent->GetDefaultRoadClass(), Location, FRotator());
    SCFL::GetObjectManager(this)->AddObjectToGrid(TempRoad);
    Temporary_ObjectToLocation.Add(Location, TempRoad);
    FixRoadAndNeighbors(TempRoad);
  }
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityRoadManager::ConvertAllTemporaryToPermanent() {
  TArray<ASimpliCityObjectBase*> RoadValues;
  Temporary_ObjectToLocation.GenerateValueArray(RoadValues);
  PermanentObjectList.Append(RoadValues);
  Temporary_ObjectToLocation.Empty();
}
//////////////////////////////////////////////////////////////////////////
void ASimpliCityRoadManager::DestroyTemporaryRoadsAtLocations(const TArray<FVector> Locations) {
  for (auto Location : Locations) {
    ASimpliCityObjectBase* RoadToDestroy = Temporary_ObjectToLocation.FindAndRemoveChecked(Location);
    SCFL::GetObjectManager(this)->RemoveObjectFromGrid(RoadToDestroy);
    FixNeighborsAtLocation(Location);
  }
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityRoadManager::DestroyAllTemporaryRoads() {
  TArray<FVector> LocKeys;
  Temporary_ObjectToLocation.GenerateKeyArray(LocKeys);
  DestroyTemporaryRoadsAtLocations(LocKeys);
  Temporary_ObjectToLocation.Empty();
}

//////////////////////////////////////////////////////////////////////////
// DESTROY
//////////////////////////////////////////////////////////////////////////
void ASimpliCityRoadManager::DestroyPermanentRoad(ASimpliCityObjectBase* Road) {
  if (ASimpliCityRoadBase* RObj = Cast<ASimpliCityRoadBase>(Road)) {
    TArray<ASimpliCityObjectBase*> neighborRoads =
        SCFL::GetObjectManager(this)->GetNeighborsOfType(RObj->GetActorLocation(), ESimpliCityObjectType::Road);
    PermanentObjectList.Remove(RObj);
    SCFL::GetObjectManager(this)->RemoveObjectFromGrid(RObj);
    for (auto nroad : neighborRoads) {
      FixRoad(Cast<ASimpliCityRoadBase>(nroad));
    }
  }
}

//////////////////////////////////////////////////////////////////////////
TArray<FVector> ASimpliCityRoadManager::GetNeighbors(FVector Location) const {
  TArray<FVector> NeighborLocs;
  TArray<ASimpliCityObjectBase*> RoadTypeNeighbors =
      SCFL::GetObjectManager(this)->GetNeighborsOfType(Location, ESimpliCityObjectType::Road);
  for (auto Object : RoadTypeNeighbors) {
    NeighborLocs.Add(Object->GetActorLocation());
  }
  return NeighborLocs;
}

//////////////////////////////////////////////////////////////////////////
// Recalculate Neighbors and Replace Static Mesh
//////////////////////////////////////////////////////////////////////////
void ASimpliCityRoadManager::SwapRoads(ASimpliCityObjectBase* OldRoad, ASimpliCityObjectBase* NewRoad) {
  // replace in either the permanent or temporary road list (depending on which contains it)
  if (PermanentObjectList.Contains(OldRoad)) {
    PermanentObjectList.Remove(OldRoad);
    PermanentObjectList.Add(NewRoad);
  } else {
    Temporary_ObjectToLocation.Add(OldRoad->GetActorLocation(), NewRoad);
  }
  // replace in the master object list
  SCFL::GetObjectManager(this)->ReplaceObjectInGrid(OldRoad, NewRoad);
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityRoadManager::FixRoad(ASimpliCityObjectBase* Road) {
  if (RoadFixerComponent == nullptr) {
    TRACE_ERROR_PRINTF(LogSimpliCity, "ERROR!! RoadFixerComponent == nullptr");
    return;
  }
  TSubclassOf<ASimpliCityRoadBase> SpawnRoadClass;
  FRotator SpawnRotation;
  RoadFixerComponent->GetRoadTypeAndRotationAtLocation(Road->GetActorLocation(), SpawnRoadClass, SpawnRotation);
  ASimpliCityRoadBase* FixedRoad = SpawnRoad(SpawnRoadClass, Road->GetActorLocation(), SpawnRotation, Road);

#if WITH_EDITOR
  FixedRoad->SetFolderPath("Roads");
#endif

  SwapRoads(Road, FixedRoad);
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityRoadManager::FixNeighborsAtLocation(FVector Location) {
  TArray<ASimpliCityObjectBase*> neighborRoads =
      SCFL::GetObjectManager(this)->GetNeighborsOfType(Location, ESimpliCityObjectType::Road);
  for (auto road : neighborRoads) {
    FixRoad(Cast<ASimpliCityRoadBase>(road));
  }
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityRoadManager::FixRoadAndNeighbors(ASimpliCityObjectBase* Road) {
  FVector roadLocation = Road->GetActorLocation();
  FixRoad(Road);
  FixNeighborsAtLocation(roadLocation);
}
