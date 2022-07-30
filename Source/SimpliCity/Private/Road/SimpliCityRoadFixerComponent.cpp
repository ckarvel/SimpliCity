// Fill out your copyright notice in the Description page of Project Settings.


#include "Road/SimpliCityRoadFixerComponent.h"

// Sets default values for this component's properties
USimpliCityRoadFixerComponent::USimpliCityRoadFixerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...
}


// Called when the game starts
void USimpliCityRoadFixerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

//TSubclassOf<ASimpliCityRoadBase> GetRoadTypeAtLocation(FVector location) {
//
//}
//
//FRotator GetRoadRotationAtLocation(FVector location) {
//	
//}
//
//void USimpliCityRoadFixerComponent::GetRoadTypeAndRotationAtLocation(FVector location, TSubclassOf<ASimpliCityRoadBase>& RoadClass,FRotator& Rotation) {
//	
//}

//bool ARoadFixer::Create4WayRoad(TArray<CellType>& adjacentCells,FVector location) {
//	UtilityLibrary::Debug("Create4wayRoad");
//
//	auto new_structure = placementManager->Create4WayRoad(location,0.0f);
//	placementManager->ModifyStructure(location,new_structure);
//	return true;
//}