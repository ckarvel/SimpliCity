// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpliCityBuildManager.h"
#include "SimpliCityFunctionLibrary.h"
#include "SimpliCityRoadBase.h"
#include "SimpliCityUtils.h"
#include "MyAStarPathFinder.h"
#include "GridManager.h"

// Sets default values
ASimpliCityBuildManager::ASimpliCityBuildManager()
  : isTrackingActive(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ASimpliCityBuildManager::BeginPlay()
{
	Super::BeginPlay();
  //AGridManager* gridMgr = USimpliCityFunctionLibrary::GetGridManager(this);
  //BuildObjects.Init(nullptr,gridMgr->GetNumRows() * gridMgr->GetNumCols());
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
    if (DoesObjectExistHere(roadLoc) == true) {
      removeRoads.Remove(roadLoc);
      TRACE_SCREENMSG_PRINTF("ObjectExists here");
    }
  }
  OnBuildRemoval.Broadcast(removeRoads.Array());

  TSet<FVector> createRoads = newPath.Difference(oldPath);
  TArray<FVector> createRoadsArr = createRoads.Array();
  // broadcast empty spaces to build on
  for (auto roadLoc : createRoadsArr) {
    if (DoesObjectExistHere(roadLoc) == true) {
      createRoads.Remove(roadLoc);
      TRACE_SCREENMSG_PRINTF("ObjectExists here");
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

bool ASimpliCityBuildManager::DoesObjectExistHere(FVector location) {
  bool objectHere = false;
  TArray<ASimpliCityRoadBase*> invalidObjects;
  for (auto p_Object : PermanentBuildObjects) {
    if (p_Object) {
      if (USimpliCityFunctionLibrary::AreLocationsEqual(p_Object->GetActorLocation(),location)) {
        objectHere = true;
        break;
      }
    }
    else {
      invalidObjects.Add(p_Object);
    }
  }
  for (auto invalidObj : invalidObjects) {
    PermanentBuildObjects.Remove(invalidObj);
  }
  return objectHere;
}

void ASimpliCityBuildManager::NotifySpawnedObject(ASimpliCityRoadBase* SpawnedObject) {
  TemporaryBuildObjects.Add(SpawnedObject);
}

void ASimpliCityBuildManager::NotifyDespawnedObject(ASimpliCityRoadBase* DespawnedObject) {
  TemporaryBuildObjects.Remove(DespawnedObject);
}

void ASimpliCityBuildManager::AddTemporaryToPermanentList() {
  for (auto object : TemporaryBuildObjects) {
    if (object) {
      object->SetNewMaterial();
    }
  }
  PermanentBuildObjects.Append(TemporaryBuildObjects);
  TemporaryBuildObjects.Empty();
}