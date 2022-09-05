// Fill out your copyright notice in the Description page of Project Settings.


#include "Road/SimpliCityRoadManager.h"
#include "Road/SimpliCityRoadFixerComponent.h"
#include "Road/SimpliCityRoadBase.h"

#include "SimpliCityObjectBase.h"

#include "MarkerManager.h"
#include "GridManager.h"

#include "SimpliCityFunctionLibrary.h"
#include "SimpliCityObjectManager.h"

// Sets default values
ASimpliCityRoadManager::ASimpliCityRoadManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ASimpliCityRoadManager::BeginPlay()
{
	Super::BeginPlay();
}


void ASimpliCityRoadManager::UpdateBuildMode(bool IsEnabled) {
	if (BuildModeEnabled == IsEnabled)
		return;

	// enabled!
	if (IsEnabled) {
		
	}
	// disabled!
	else {

	}
}

void ASimpliCityRoadManager::StartPlacingRoad(FVector Location) {
	oldPath.Empty();
	startLocation = USimpliCityFunctionLibrary::GetGridManager(this)->LocationToCenter(Location);
	BuildModeEnabled = true;
}

void ASimpliCityRoadManager::UpdatePath(FVector Location) {
	if (BuildModeEnabled == false) {
		return;
	}

	AGridManager* gridMgr = USimpliCityFunctionLibrary::GetGridManager(this);
	FVector currentLocation = gridMgr->LocationToCenter(Location);
	if (lastLocation == currentLocation)
		return; // mouse on same tile as before, so exit
	lastLocation = currentLocation;

	// calculate new path between start & given location
	TSet<FVector> newPath = TSet<FVector>(USimpliCityFunctionLibrary::GetPathBetween(gridMgr,startLocation,currentLocation));
	TSet<FVector> removeRoads = oldPath.Difference(newPath);
	TArray<FVector> removeRoadsArr = removeRoads.Array();
	ASimpliCityObjectManager* objectMgr = USimpliCityFunctionLibrary::GetObjectManager(this);
	for (auto roadLoc : removeRoadsArr) {
		// if this location is not a temporary road, then we don't want to remove this road.
		if (TemporaryRoadLocMap.Contains(roadLoc) == false) {
			removeRoads.Remove(roadLoc);
			//TRACE_SCREENMSG_PRINTF("Don't remove: Permanent object exists here!");
		}
	}

	DestroyTemporaryRoadsAtLocations(removeRoads.Array());

	TSet<FVector> createRoads = newPath.Difference(oldPath);
	TArray<FVector> createRoadsArr = createRoads.Array();
	// temporary roads to create
	for (auto roadLoc : createRoadsArr) {
		if (objectMgr->DoesObjectExistHere(roadLoc) == true) {
			createRoads.Remove(roadLoc);
			//TRACE_SCREENMSG_PRINTF("Don't create: Permanent object exists here!");
		}
	}
	CreateTemporaryRoadsAtLocations(createRoads.Array());
	
	oldPath = newPath;
}

void ASimpliCityRoadManager::FinishBuildingPath() {
	oldPath.Empty();
	startLocation = FVector();
	BuildModeEnabled = false;
	ConvertAllTemporaryToPermanent();
	TArray<UObject*> NewRoads;
	NewRoads.Append(PermanentRoadList);
	AgentMarkerGraph->UpdateGraph(NewRoads);
}

void ASimpliCityRoadManager::CancelBuildingPath() {
	oldPath.Empty();
	startLocation = FVector();
	BuildModeEnabled = false;
	DestroyAllTemporaryRoads();
}

void ASimpliCityRoadManager::CreateTemporaryRoadsAtLocations(const TArray<FVector>& Locations) {
	for (auto Location : Locations) {
		ASimpliCityRoadBase* TempRoad = GetWorld()->SpawnActor<ASimpliCityRoadBase>(RoadFixerComponent->GetDefaultRoadClass(), Location, FRotator());
		USimpliCityFunctionLibrary::GetObjectManager(this)->AddObjectToGrid(TempRoad);
		TemporaryRoadLocMap.Add(Location,TempRoad);
		FixRoadAndNeighbors(TempRoad);
	}
}

void ASimpliCityRoadManager::ConvertAllTemporaryToPermanent() {
	TArray<ASimpliCityRoadBase*> RoadValues;
	TemporaryRoadLocMap.GenerateValueArray(RoadValues);
	PermanentRoadList.Append(RoadValues);
	TemporaryRoadLocMap.Empty();
}

void ASimpliCityRoadManager::DestroyTemporaryRoadsAtLocations(const TArray<FVector> Locations) {
		for (auto Location : Locations) {
			ASimpliCityRoadBase* RoadToDestroy = TemporaryRoadLocMap.FindAndRemoveChecked(Location);
			USimpliCityFunctionLibrary::GetObjectManager(this)->RemoveObjectFromGrid(RoadToDestroy);
			FixNeighborsAtLocation(Location);
		}
}

void ASimpliCityRoadManager::DestroyAllTemporaryRoads() {
	TArray<FVector> LocKeys;
	TemporaryRoadLocMap.GenerateKeyArray(LocKeys);
	DestroyTemporaryRoadsAtLocations(LocKeys);
	TemporaryRoadLocMap.Empty();
}

TArray<FVector> ASimpliCityRoadManager::GetNeighbors(FVector Location) const {
	TArray<FVector> NeighborLocs;
	TArray<ASimpliCityObjectBase*> RoadTypeNeighbors = USimpliCityFunctionLibrary::GetObjectManager(this)->GetNeighborsOfType(Location,ESimpliCityObjectType::Road);
	for (auto Object : RoadTypeNeighbors) {
		NeighborLocs.Add(Object->GetActorLocation());
	}
	return NeighborLocs;
}

void ASimpliCityRoadManager::SwapRoads(ASimpliCityObjectBase* OldRoad,ASimpliCityObjectBase* NewRoad) {
	// replace in either the permanent or temporary road list (depending on which contains it)
	if (PermanentRoadList.Contains(OldRoad)) {
		PermanentRoadList.Remove(Cast<ASimpliCityRoadBase>(OldRoad));
		PermanentRoadList.Add(Cast<ASimpliCityRoadBase>(NewRoad));
	}
	else {
		TemporaryRoadLocMap.Add(OldRoad->GetActorLocation(),Cast<ASimpliCityRoadBase>(NewRoad));
	}
	
	// replace in the master object list
	USimpliCityFunctionLibrary::GetObjectManager(this)->ReplaceObjectInGrid(OldRoad,NewRoad);
}

void ASimpliCityRoadManager::FixRoad(ASimpliCityObjectBase* Road) {
	check(RoadFixerComponent != nullptr);
	TSubclassOf<ASimpliCityRoadBase> SpawnRoadClass;
	FRotator SpawnRotation;
	RoadFixerComponent->GetRoadTypeAndRotationAtLocation(Road->GetActorLocation(),SpawnRoadClass,SpawnRotation);
	ASimpliCityRoadBase* FixedRoad = GetWorld()->SpawnActor<ASimpliCityRoadBase>(SpawnRoadClass,Road->GetActorLocation(),SpawnRotation);
	SwapRoads(Road,FixedRoad);
}

void ASimpliCityRoadManager::FixNeighborsAtLocation(FVector Location) {
	TArray<ASimpliCityObjectBase*> neighborRoads = USimpliCityFunctionLibrary::GetObjectManager(this)->GetNeighborsOfType(Location,ESimpliCityObjectType::Road);
	for (auto road : neighborRoads) {
		FixRoad(Cast<ASimpliCityRoadBase>(road));
	}
}

void ASimpliCityRoadManager::FixRoadAndNeighbors(ASimpliCityObjectBase* Road) {
	FVector roadLocation = Road->GetActorLocation();
	FixRoad(Road);
	FixNeighborsAtLocation(roadLocation);
}