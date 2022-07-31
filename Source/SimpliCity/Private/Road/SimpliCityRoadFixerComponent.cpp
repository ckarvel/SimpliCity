// Fill out your copyright notice in the Description page of Project Settings.


#include "Road/SimpliCityRoadFixerComponent.h"

#include "Builder/SimpliCityBuildManager.h"
#include "Builder/SimpliCityBuildObjectBase.h"
#include "Builder/SimpliCityBuildObjectEnum.h"

#include "Tools/SimpliCityFunctionLibrary.h"
#include "Tools/SimpliCityUtils.h"

#include "GridManager.h"

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
void USimpliCityRoadFixerComponent::GetRoadTypeAndRotationAtLocation(FVector location, TSubclassOf<ASimpliCityRoadBase>& OutRoadClass,FRotator& OutRotation) {
	TArray<ASimpliCityBuildObjectBase*> neighborRoads = USimpliCityFunctionLibrary::GetBuildManager(this)->GetAllNeighbors_Unsafe(location);
	OutRotation = FRotator(0,0,0); // zero it out just in case
	int roadCount = 0;
	for (auto road : neighborRoads) {
		if (road != nullptr && road->Type() == ESimpliCityBuildObjectEnum::BuildObject_Road) {
				roadCount +=1;
		}
	}
  if (roadCount > 3)
		Set4WayRoadInfo(location, neighborRoads, OutRoadClass, OutRotation);
  else if (roadCount > 2)
		Set3WayRoadInfo(location,neighborRoads,OutRoadClass,OutRotation);
  else if (roadCount > 1) {
    if (SetStraightRoadInfo(location,neighborRoads,OutRoadClass,OutRotation))
      return;
		SetCornerWayRoadInfo(location,neighborRoads,OutRoadClass,OutRotation);
  } else
		SetDeadEndRoadInfo(location,neighborRoads,OutRoadClass,OutRotation);
}

void USimpliCityRoadFixerComponent::Set4WayRoadInfo(FVector location, TArray<ASimpliCityBuildObjectBase*> neighbors, TSubclassOf<ASimpliCityRoadBase>& OutRoadClass,FRotator& OutRotation) {
	TRACE_SCREENMSG_PRINTF("Set4WayRoadInfo");
	OutRoadClass = Road4WayClass;
	OutRotation.Yaw = 0.0;
}

bool USimpliCityRoadFixerComponent::IsRoad(ASimpliCityBuildObjectBase* obj) {
	bool isRoad = false;
	if (obj != nullptr && obj->Type() == ESimpliCityBuildObjectEnum::BuildObject_Road)
		isRoad = true;
	return isRoad;
}

// 0 - left, right, bottom (0, 1, 3)
// 90 - left, top, bottom (0, 2, 3)
// 180 - left, right, top (0, 1, 2)
// 270 - right, top, bottom (1, 2, 3)
void USimpliCityRoadFixerComponent::Set3WayRoadInfo(FVector location,TArray<ASimpliCityBuildObjectBase*> neighbors,TSubclassOf<ASimpliCityRoadBase>& OutRoadClass,FRotator& OutRotation) {
	TRACE_SCREENMSG_PRINTF("Set3WayRoadInfo");
	OutRoadClass = Road3WayClass;

	if (IsRoad(neighbors[0]) && IsRoad(neighbors[1]) && IsRoad(neighbors[3]))
		OutRotation.Yaw = 0.0;
	else if (IsRoad(neighbors[0]) && IsRoad(neighbors[2]) && IsRoad(neighbors[3]))
		OutRotation.Yaw = 90.0;
	else if (IsRoad(neighbors[0]) && IsRoad(neighbors[1]) && IsRoad(neighbors[2]))
		OutRotation.Yaw = 180.0;
	else
		OutRotation.Yaw = 270.0;
}

// 0 - top, bottom (2, 3)
// 90 - left, right (0, 1)
bool USimpliCityRoadFixerComponent::SetStraightRoadInfo(FVector location,TArray<ASimpliCityBuildObjectBase*> neighbors,TSubclassOf<ASimpliCityRoadBase>& OutRoadClass,FRotator& OutRotation) {
	TRACE_SCREENMSG_PRINTF("SetStraightRoadInfo");
	bool IsStraightRoad = false;

	if (IsRoad(neighbors[2]) && IsRoad(neighbors[3])) {
		OutRoadClass = RoadStraightClass;
		OutRotation.Yaw = 0.0;
		IsStraightRoad = true;
	}
	else if (IsRoad(neighbors[0]) && IsRoad(neighbors[1])) {
		OutRoadClass = RoadStraightClass;
		OutRotation.Yaw = 90.0;
		IsStraightRoad = true;
	}

	// notify if this road was indeed straight or not
	return IsStraightRoad;
}

// 0 - right, top (1, 2)
// 90 - right, bottom (1, 3)
// 180 - left, bottom (0, 3)
// 270 - left, top (0, 2)
void USimpliCityRoadFixerComponent::SetCornerWayRoadInfo(FVector location,TArray<ASimpliCityBuildObjectBase*> neighbors,TSubclassOf<ASimpliCityRoadBase>& OutRoadClass,FRotator& OutRotation) {
	TRACE_SCREENMSG_PRINTF("SetCornerWayRoadInfo");
	OutRoadClass = RoadCornerClass;

	if (IsRoad(neighbors[1]) && IsRoad(neighbors[2]))
		OutRotation.Yaw = 0.0;
	else if (IsRoad(neighbors[1]) && IsRoad(neighbors[3]))
		OutRotation.Yaw = 90.0;
	else if (IsRoad(neighbors[0]) && IsRoad(neighbors[3]))
		OutRotation.Yaw = 180.0;
	else
		OutRotation.Yaw = 270.0;
}

// 0 - bottom (3)
// 90 - left (0)
// 180 - top (2)
// 270 - right (1)
void USimpliCityRoadFixerComponent::SetDeadEndRoadInfo(FVector location,TArray<ASimpliCityBuildObjectBase*> neighbors,TSubclassOf<ASimpliCityRoadBase>& OutRoadClass,FRotator& OutRotation) {
	TRACE_SCREENMSG_PRINTF("SetDeadEndRoadInfo");
	OutRoadClass = RoadDeadEndClass;
	if (IsRoad(neighbors[3]))
		OutRotation.Yaw = 0.0;
	else if (IsRoad(neighbors[0]))
		OutRotation.Yaw = 90.0;
	else if (IsRoad(neighbors[2]))
		OutRotation.Yaw = 180.0;
	else
		OutRotation.Yaw = 270.0;
}