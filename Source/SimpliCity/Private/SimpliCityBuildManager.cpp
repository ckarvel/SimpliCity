// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpliCityBuildManager.h"
#include "SimpliCityFunctionLibrary.h"
#include "MyAStarPathFinder.h"
#include "GridManager.h"

// Sets default values
ASimpliCityBuildManager::ASimpliCityBuildManager()
  : isTrackingActive(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASimpliCityBuildManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASimpliCityBuildManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// make sure location is adjusted by gridmanager before passing!
void ASimpliCityBuildManager::StartTrackingBuildPath(FVector currentLocation) {
  startLocation = currentLocation;
	oldPath.Empty();
	oldPath.Add(startLocation);
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

  // broadcast removed roads
  TSet<FVector> removeRoads = oldPath.Difference(newPath);
  OnBuildRemoval.Broadcast(removeRoads.Array());

  // broadcast new roads
  TSet<FVector> createRoads = newPath.Difference(oldPath);
  OnBuildCreation.Broadcast(createRoads.Array());

  oldPath = newPath;
}

void ASimpliCityBuildManager::FinishTrackingBuildPath() {
  oldPath.Empty();
  startLocation = FVector();
  isTrackingActive = false;
}