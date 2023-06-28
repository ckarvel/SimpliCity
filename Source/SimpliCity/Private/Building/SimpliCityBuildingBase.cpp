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
  ASimpliCityObjectBase::SetNewLocation(Location);
  auto Neighbors = ObjectManager->GetNeighborsOfType(Location, ESimpliCityObjectType::Road);
  if (Neighbors.Num() > 0) {
    RoadConnection = Cast<ASimpliCityRoadBase>(Neighbors[0]);
  } else {
    bIsValidPlacement = false;
  }
  OnNewLocation(bIsValidPlacement);
}
