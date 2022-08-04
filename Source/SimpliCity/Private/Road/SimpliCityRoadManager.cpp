// Fill out your copyright notice in the Description page of Project Settings.


#include "Road/SimpliCityRoadManager.h"
#include "Road/SimpliCityRoadFixerComponent.h"
#include "Road/SimpliCityRoadBase.h"

#include "Builder/SimpliCityBuildObjectEnum.h"
#include "Builder/SimpliCityBuildObjectBase.h"
#include "Builder/SimpliCityBuildManager.h"

#include "MarkerManager.h"

#include "Tools/SimpliCityFunctionLibrary.h"

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
	VehicleMarkerGraph = GetWorld()->SpawnActor<AMarkerManager>(AMarkerManager::StaticClass());
}

// Called every frame
void ASimpliCityRoadManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASimpliCityRoadManager::SwapRoads(ASimpliCityBuildObjectBase* OldRoad,ASimpliCityBuildObjectBase* NewRoad) {
	USimpliCityFunctionLibrary::GetBuildManager(this)->NotifyReplaceObject(OldRoad, NewRoad);
	RoadList.Remove(Cast<ASimpliCityRoadBase>(OldRoad));
	RoadList.Add(Cast<ASimpliCityRoadBase>(NewRoad));
}

void ASimpliCityRoadManager::FixRoad(ASimpliCityBuildObjectBase* Road) {
	check(RoadFixerComponent != nullptr);
	TSubclassOf<ASimpliCityRoadBase> SpawnRoadClass;
	FRotator SpawnRotation;
	RoadFixerComponent->GetRoadTypeAndRotationAtLocation(Road->GetActorLocation(),SpawnRoadClass,SpawnRotation);
	ASimpliCityRoadBase* FixedRoad = GetWorld()->SpawnActor<ASimpliCityRoadBase>(SpawnRoadClass,Road->GetActorLocation(),SpawnRotation);
	SwapRoads(Road,FixedRoad);
	Road->Destroy();
}

void ASimpliCityRoadManager::FixNeighborsOfRoad(ASimpliCityBuildObjectBase* Road) {
	FVector roadLocation = Road->GetActorLocation();
	TArray<ASimpliCityBuildObjectBase*> neighborRoads = USimpliCityFunctionLibrary::GetBuildManager(this)->GetNeighborsOfType(roadLocation,ESimpliCityBuildObjectEnum::BuildObject_Road);
	for (auto road : neighborRoads) {
		FixRoad(Cast<ASimpliCityRoadBase>(road));
	}
}

void ASimpliCityRoadManager::FixRoadAndNeighbors(ASimpliCityBuildObjectBase* Road) {
	FVector roadLocation = Road->GetActorLocation();
	FixRoad(Road);
	FixNeighborsOfRoad(Road);
}