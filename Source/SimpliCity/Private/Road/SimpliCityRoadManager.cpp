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
void ASimpliCityRoadManager::Enable(ESimpliCityResourceType _TypeId) {
  ASimpliCityBaseManager::Enable(_TypeId);
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
  TSet<FVector> createRoads = newPath.Difference(oldPath);
  for (auto const& roadLoc : createRoads.Array()) {
    if (ObjectManager->DoesObjectExistHere(roadLoc) == true) {
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
  int index = 0;
  int length = Locations.Num();
  for (int i = 0; i < length;  i++) {
    auto Location = Locations[i];
    ASimpliCityObjectBase* TempRoad = SpawnObjectOfType(DefaultBlueprintClass, Location, FRotator(), ResourceType);
    ASimpliCityRoadBase* Road = Cast<ASimpliCityRoadBase>(TempRoad);
    // todo: a shadow grid so this road isn't valid until its placed
    //ObjectManager->AddObjectToGrid(TempRoad);
    Temporary_ObjectToLocation.Add(Location, TempRoad);
    FixRoadAndNeighbors(TempRoad);
  }
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityRoadManager::ConvertAllTemporaryToPermanent() {
  for (auto Road : Temporary_ObjectToLocation) {
    auto RoadLoc = Road.Key;
    auto NewRoad = Road.Value;
    if (auto OldRoad = ObjectManager->GetObjectAtLocation(RoadLoc)) {
      PermanentObjectList.Remove(OldRoad);
      ObjectManager->RemoveObjectFromGrid(OldRoad);
    }
    PermanentObjectList.Add(NewRoad);
    ObjectManager->AddObjectToGrid(NewRoad);
    NewRoad->OnObjectPlaced();
  }
  Temporary_ObjectToLocation.Empty();
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityRoadManager::DestroyTemporaryRoadsAtLocations(const TArray<FVector> Locations) {
  for (auto const& Location : Locations) {
    ASimpliCityObjectBase* RoadToDestroy = Temporary_ObjectToLocation.FindAndRemoveChecked(Location);
    RoadToDestroy->Destroy();
    FixNeighborsAtLocation(Location);
  }
}

//////////////////////////////////////////////////////////////////////////
TArray<FVector> ASimpliCityRoadManager::GetNeighbors(FVector Location) const {
  TArray<FVector> NeighborLocs;
  TArray<ASimpliCityObjectBase*> RoadTypeNeighbors =
      ObjectManager->GetNeighborsOfType(Location, ESimpliCityObjectType::Road);
  for (auto Object : RoadTypeNeighbors) {
    NeighborLocs.Add(Object->GetActorLocation());
  }
  return NeighborLocs;
}

//////////////////////////////////////////////////////////////////////////
TArray<ASimpliCityObjectBase*> ASimpliCityRoadManager::GetTempAndPermNeighbors(FVector Location) const {
  // get all perma neighbors, so 4 in total even if some are nullptr
  TArray<ASimpliCityObjectBase*> neighborRoads = ObjectManager->GetAllNeighbors(Location);
  int length = neighborRoads.Num();

  // perma neighbor list is sorted so we use the index to find the location
  int locIndex = GridManager->LocationToIndex(Location);
  TArray<int> OutIndexes;
  GridManager->GetAllNeighborIndexes(locIndex, OutIndexes);

  for (int i = 0; i < length; i++) {
    // check if a temp neighbor exists, if so replace (temp has priority as neighbor here)
    int index = OutIndexes[i];
    if (GridManager->IsIndexValid(index) == false)
      continue;
    // store the temporary object at this spot
    FVector indexLoc = GridManager->IndexToLocation(index, true);
    if (auto temproad = Temporary_ObjectToLocation.Find(indexLoc)) {
      neighborRoads[i] = *temproad;
    }
  }
  return neighborRoads;
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityRoadManager::FixRoad(ASimpliCityObjectBase* Road) {
  if (RoadFixerComponent == nullptr) {
    TRACE_ERROR_PRINTF(LogSimpliCity, "ERROR!! RoadFixerComponent == nullptr");
    return;
  }
  if (Road == nullptr || Road->Type() != ESimpliCityObjectType::Road)
    return;

  TSubclassOf<ASimpliCityRoadBase> OutRoadClass;
  FVector SpawnLocation = Road->GetActorLocation();
  FRotator OutRotation;

  TArray<ASimpliCityObjectBase*> neighborRoads = GetTempAndPermNeighbors(SpawnLocation);
  RoadFixerComponent->GetRoadTypeAndRotationAtLocation(SpawnLocation, neighborRoads, OutRoadClass, OutRotation);

  // if this road has same class and rotation...
  if (Road->GetClass() == OutRoadClass && Road->GetActorRotation().Equals(OutRotation)) {
    TRACE_LOG_PRINTF(LogSimpliCity, "No need to fix road");
    return;
  }

  ASimpliCityObjectBase* FixedRoad = SpawnObjectOfType(OutRoadClass, SpawnLocation, OutRotation, ResourceType);

#if WITH_EDITOR
  FixedRoad->SetFolderPath("Roads");
#endif

  // if old road is temporary, delete it
  if (auto TempRoad = Temporary_ObjectToLocation.Find(SpawnLocation)) {
    // There might be a permanent and temporary road on this tile. (grid vs shadow grid)
    // So remove TempRoad not Road. Road may be permanent
    (*TempRoad)->Destroy();
  }
  // else don't delete it, temp will just overlay the perm one
  Temporary_ObjectToLocation.Add(SpawnLocation, FixedRoad);
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityRoadManager::FixNeighborsAtLocation(FVector Location) {
  TArray<ASimpliCityObjectBase*> neighborRoads = GetTempAndPermNeighbors(Location);
  for (auto road : neighborRoads) {
    FixRoad(road);
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
    if (Object == nullptr || Object->Type() != ESimpliCityObjectType::Road)
      continue; // if null or not a road, ignore

    FVector ObjectLocation = Object->GetActorLocation();
    TArray<ASimpliCityObjectBase*> neighborRoads = GetTempAndPermNeighbors(ObjectLocation);
    if (PermanentObjectList.Contains(Object)) {
      ASimpliCityBaseManager::DestroyObject(Object); // destroys and removes from grid/list
    } else {
      // destroy and remove from temp list
      Temporary_ObjectToLocation.Remove(ObjectLocation);
      Object->Destroy();
    }
    for (auto nroad : neighborRoads) {
      // if neighbor is not in the removal list, fix it
      if (ObjectList.Contains(nroad) == false)
        FixRoad(nroad);
    }
  }
}
