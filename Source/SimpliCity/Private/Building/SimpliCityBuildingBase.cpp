// Fill out your copyright notice in the Description page of Project Settings.

#include "Building/SimpliCityBuildingBase.h"

#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "MarkerComponent.h"
#include "MarkerManager.h"
#include "Road/SimpliCityRoadBase.h"
#include "SimpliCityFunctionLibrary.h"
#include "SimpliCityObjectManager.h"

using SCFL = USimpliCityFunctionLibrary;

ASimpliCityBuildingBase::ASimpliCityBuildingBase()
    : YawOffset(-90) {
  ObjectType = ESimpliCityObjectType::Building;
}

void ASimpliCityBuildingBase::SetNewLocation(FVector Location) {
  ASimpliCityObjectBase::SetNewLocation(Location);
  auto Neighbors = ObjectManager->GetNeighborsOfType(Location, ESimpliCityObjectType::Road);
  if (Neighbors.Num() > 0) {
    RoadConnection = Cast<ASimpliCityRoadBase>(Neighbors[0]);
    FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(Location, RoadConnection->GetActorLocation());
    Rotation.Yaw += YawOffset;
    SetActorRotation(Rotation);
  } else {
    bIsValidPlacement = false;
  }
  OnNewLocation(bIsValidPlacement);
}
