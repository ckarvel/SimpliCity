// Fill out your copyright notice in the Description page of Project Settings.

#include "Building/SimpliCityBuildingBase.h"

#include "Components/StaticMeshComponent.h"
#include "MarkerComponent.h"
#include "MarkerManager.h"
#include "Road/SimpliCityRoadBase.h"
#include "SimpliCityFunctionLibrary.h"
#include "SimpliCityObjectManager.h"

using SCFL = USimpliCityFunctionLibrary;

ASimpliCityBuildingBase::ASimpliCityBuildingBase() {
  ObjectType = ESimpliCityObjectType::Building;
}

void ASimpliCityBuildingBase::SetNewLocation(FVector Location) {
  auto* ObjectMgr = SCFL::GetObjectManager(this);
  auto Neighbors = ObjectMgr->GetNeighborsOfType(Location, ESimpliCityObjectType::Road);
  // if location is occupied set material red for error
  if (ObjectMgr->DoesObjectExistHere(Location) || Neighbors.Num() <= 0) {
    return SetNewMaterial(ErrorMaterial);
  }

  RoadConnection = Cast<ASimpliCityRoadBase>(Neighbors[0]);
}
