// Fill out your copyright notice in the Description page of Project Settings.


#include "Builder/SimpliCityBuildManager.h"
#include "Builder/SimpliCityBuildObjectBase.h"
#include "Builder/SimpliCityBuildObjectGrid.h"

#include "MyAStarPathFinder.h"
#include "GridManager.h"

#include "Tools/SimpliCityFunctionLibrary.h"
#include "Tools/SimpliCityUtils.h"


// Sets default values
ASimpliCityBuildManager::ASimpliCityBuildManager()
  : isTrackingActive(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
  ObjectGridComponent = CreateDefaultSubobject<USimpliCityBuildObjectGrid>(TEXT("Object Grid"));
}

// Called when the game starts or when spawned
void ASimpliCityBuildManager::BeginPlay()
{
	Super::BeginPlay();
}

// make sure location is adjusted by gridmanager before passing!
void ASimpliCityBuildManager::StartTrackingBuildPath(FVector currentLocation) {
  startLocation = currentLocation;
	oldPath.Empty();
	oldPath.Add(startLocation);
  // broadcast the starting position
  OnBuildCreation.Broadcast(oldPath.Array());
  isTrackingActive = true;
}

// make sure location is adjusted by gridmanager before passing!
void ASimpliCityBuildManager::TrackBuildPath(FVector currentLocation) {
  if (isTrackingActive == false) {
    return;
  }
	if (lastLocation == currentLocation)
		return; // mouse on same tile as before, so exit
	lastLocation = currentLocation;

  // calculate new path between start & given location
  AGridManager* gridMgr = USimpliCityFunctionLibrary::GetGridManager(this);
  TSet<FVector> newPath = TSet<FVector>(MyAStarPathFinder::AStarSearch(gridMgr,startLocation,currentLocation));

  // broadcast non-permanent roads to remove
  TSet<FVector> removeRoads = oldPath.Difference(newPath);
  TArray<FVector> removeRoadsArr = removeRoads.Array();
  for (auto roadLoc : removeRoadsArr) {
    if (ASimpliCityBuildObjectBase* obj = GetObjectAtLocation(roadLoc)) {
      if (TemporaryBuildObjects.Contains(obj) == false) {
        removeRoads.Remove(roadLoc);
      }
      else {
        TRACE_SCREENMSG_PRINTF("Don't remove: Permanent object exists here!");
      }
    }
  }
  OnBuildRemoval.Broadcast(removeRoads.Array());

  TSet<FVector> createRoads = newPath.Difference(oldPath);
  TArray<FVector> createRoadsArr = createRoads.Array();
  // broadcast empty spaces to build on
  for (auto roadLoc : createRoadsArr) {
    if (ASimpliCityBuildObjectBase* obj = GetObjectAtLocation(roadLoc)) {
      if (TemporaryBuildObjects.Contains(obj) == false) {
        createRoads.Remove(roadLoc);
      } else {
        TRACE_SCREENMSG_PRINTF("Don't create: Permanent object exists here!");
      }
    }
  }
  OnBuildCreation.Broadcast(createRoads.Array());

  oldPath = newPath;
}

void ASimpliCityBuildManager::FinishTrackingBuildPath() {
  oldPath.Empty();
  startLocation = FVector();
  isTrackingActive = false;
  AddTemporaryToPermanentList();
}

void ASimpliCityBuildManager::AddTemporaryToPermanentList() {
  // todo: remove debug stuff
  for (auto object : TemporaryBuildObjects) {
    if (object) {
      object->SetNewMaterial();
    }
  }
  TemporaryBuildObjects.Empty();
}

void ASimpliCityBuildManager::NotifySpawnedObject(ASimpliCityBuildObjectBase* SpawnedObject) {
  TemporaryBuildObjects.Add(SpawnedObject);
  ObjectGridComponent->NotifySpawnedObject(SpawnedObject);
}

void ASimpliCityBuildManager::NotifyDespawnedObject(ASimpliCityBuildObjectBase* DespawnedObject) {
  TemporaryBuildObjects.Remove(DespawnedObject);
  ObjectGridComponent->NotifyDespawnedObject(DespawnedObject);
}

void ASimpliCityBuildManager::NotifyDespawnedObjectAtLocation(FVector location) {
  if (ASimpliCityBuildObjectBase* obj = GetObjectAtLocation(location)) {
    TemporaryBuildObjects.Remove(obj);
  }
  ObjectGridComponent->NotifyDespawnedObjectAtLocation(location);
}

ASimpliCityBuildObjectBase* ASimpliCityBuildManager::GetObjectAtLocation(FVector location) {
  return ObjectGridComponent->GetObjectAtLocation(location);
}