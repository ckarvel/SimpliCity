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
    if (DoesObjectExistHere(roadLoc) && !TemporaryBuildLocations.Contains(roadLoc)) {
      removeRoads.Remove(roadLoc);
      TRACE_SCREENMSG_PRINTF("Don't remove: Permanent object exists here!");
    }
  }
  OnBuildRemoval.Broadcast(removeRoads.Array());

  TSet<FVector> createRoads = newPath.Difference(oldPath);
  TArray<FVector> createRoadsArr = createRoads.Array();
  // broadcast empty spaces to build on
  for (auto roadLoc : createRoadsArr) {
    if (DoesObjectExistHere(roadLoc) && !TemporaryBuildLocations.Contains(roadLoc)) {
      createRoads.Remove(roadLoc);
      TRACE_SCREENMSG_PRINTF("Don't create: Permanent object exists here!");
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
  TemporaryBuildLocations.Empty();
}

bool ASimpliCityBuildManager::DoesObjectExistHere(FVector Location) {
  return ObjectGridComponent->DoesObjectExistHere(Location);
}

void ASimpliCityBuildManager::NotifySpawnedObject(ASimpliCityBuildObjectBase* SpawnedObject) {
  TemporaryBuildObjects.Add(SpawnedObject);
  TemporaryBuildLocations.Add(SpawnedObject->GetActorLocation());
  ObjectGridComponent->NotifySpawnedObject(SpawnedObject);
}

void ASimpliCityBuildManager::NotifyDespawnedObject(ASimpliCityBuildObjectBase* DespawnedObject) {
  TemporaryBuildObjects.Remove(DespawnedObject);
  TemporaryBuildLocations.Remove(DespawnedObject->GetActorLocation());
  ObjectGridComponent->NotifyDespawnedObject(DespawnedObject);
}

void ASimpliCityBuildManager::NotifyDespawnedObjectAtLocation(FVector location) {
  if (ASimpliCityBuildObjectBase* obj = GetObjectAtLocation(location)) {
    TemporaryBuildObjects.Remove(obj);
  }
  TemporaryBuildLocations.Remove(location);
  ObjectGridComponent->NotifyDespawnedObjectAtLocation(location);
}

ASimpliCityBuildObjectBase* ASimpliCityBuildManager::GetObjectAtLocation(FVector location) {
  return ObjectGridComponent->GetObjectAtLocation(location);
}

TArray<ASimpliCityBuildObjectBase*> ASimpliCityBuildManager::GetNeighborsOfType(FVector location,TEnumAsByte<ESimpliCityBuildObjectEnum> buildType) {
  return ObjectGridComponent->GetNeighborsOfType(location, buildType);
}

TArray<ASimpliCityBuildObjectBase*> ASimpliCityBuildManager::GetAllNeighbors_Unsafe(FVector location) {
  return ObjectGridComponent->GetAllNeighbors_Unsafe(location);
}

void ASimpliCityBuildManager::NotifyReplaceObject(ASimpliCityBuildObjectBase* OldObj, ASimpliCityBuildObjectBase* NewObj) {
  ObjectGridComponent->NotifySpawnedObject(NewObj);
  TemporaryBuildObjects.Add(NewObj);
  TemporaryBuildObjects.Remove(OldObj);
}
