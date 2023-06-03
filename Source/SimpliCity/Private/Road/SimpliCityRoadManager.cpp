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
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityRoadManager::Enable(UTexture2D* NewIcon) {
  ASimpliCityBaseManager::Enable(NewIcon);
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityRoadManager::Disable() {
  ASimpliCityBaseManager::Disable();
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityRoadManager::StartBuilding() {
  ASimpliCityBaseManager::StartBuilding();
  oldPath.Empty();
}

//////////////////////////////////////////////////////////////////////////
bool ASimpliCityRoadManager::UpdateBuilding(FVector Location) {
  if (ASimpliCityBaseManager::UpdateBuilding(Location) == false) {
    return false;
  }

  // calculate new path between start & given location
  TSet<FVector> newPath = TSet<FVector>(SCFL::GetPathBetween(GridManager, StartLocation, LastLocation));
  TSet<FVector> removeRoads = oldPath.Difference(newPath);

  // remove temporary roads (not permanent!) that aren't on the path anymore
  for (auto const& roadLoc : removeRoads.Array()) {
    if (Temporary_ObjectToLocation.Contains(roadLoc) == false) {
      removeRoads.Remove(roadLoc);
    }
  }
  DestroyTemporaryRoadsAtLocations(removeRoads.Array());

  // create temporary roads (on available spots!) for new path
  ASimpliCityObjectManager* objectMgr = SCFL::GetObjectManager(this);
  TSet<FVector> createRoads = newPath.Difference(oldPath);
  for (auto const& roadLoc : createRoads.Array()) {
    if (objectMgr->DoesObjectExistHere(roadLoc) == true) {
      createRoads.Remove(roadLoc);
    }
  }
  CreateTemporaryRoadsAtLocations(createRoads.Array());

  oldPath = newPath;
  return true;
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
  TArray<ASimpliCityObjectBase*> RoadObjects;
  Temporary_ObjectToLocation.GenerateValueArray(RoadObjects);
  DestroyObjects(RoadObjects);
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityRoadManager::CreateTemporaryRoadsAtLocations(const TArray<FVector>& Locations) {
  for (auto const& Location : Locations) {
    ASimpliCityObjectBase* TempRoad = SpawnObjectOfType(DefaultBlueprintClass, Location, FRotator(), BuildIcon);
    // todo: a shadow grid so this road isn't valid until its placed
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
  for (auto const& Location : Locations) {
    ASimpliCityObjectBase* RoadToDestroy = Temporary_ObjectToLocation.FindAndRemoveChecked(Location);
    SCFL::GetObjectManager(this)->RemoveObjectFromGrid(RoadToDestroy);
    FixNeighborsAtLocation(Location);
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
  FVector SpawnLocation = Road->GetActorLocation();
  FRotator SpawnRotation;
  RoadFixerComponent->GetRoadTypeAndRotationAtLocation(SpawnLocation, SpawnRoadClass, SpawnRotation);
  // ASimpliCityRoadBase* FixedRoad = SpawnRoad(SpawnRoadClass, Road->GetActorLocation(), SpawnRotation, Road);
  ASimpliCityObjectBase* FixedRoad = SpawnObjectOfType(SpawnRoadClass, SpawnLocation, SpawnRotation, Road->BuildIcon);

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
