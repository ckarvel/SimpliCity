// Fill out your copyright notice in the Description page of Project Settings.

#include "Road/SimpliCityRoadBase.h"

#include "Components/StaticMeshComponent.h"
#include "MarkerComponent.h"
#include "MarkerManager.h"
#include "SimpliCityFunctionLibrary.h"
#include "SimpliCityObjectManager.h"
#include "Road/SimpliCityRoadManager.h"
#include "GridManager.h"

using SCFL = USimpliCityFunctionLibrary;

ASimpliCityRoadBase::ASimpliCityRoadBase() {
  ObjectType = ESimpliCityObjectType::Road;
}

void ASimpliCityRoadBase::BeginPlay() {
  Super::BeginPlay();
  RoadManager = SCFL::GetRoadManager(this);
}

void ASimpliCityRoadBase::SetNewLocation(FVector Location) {
  // Get permanent road neighbors
  TArray<ASimpliCityObjectBase*> Neighbors = ObjectManager->GetNeighborsOfType(Location, ESimpliCityObjectType::Road);

  TArray<ASimpliCityObjectBase*> TempNeighbors;
  // Get temporary road neighbors
  TArray<FVector> NeighborLocs = GridManager->GetNeighbors(Location);
  for (auto Loc : NeighborLocs) {
    auto Object = RoadManager->GetTemporaryObjectAtLocation(Loc);
    if (Object != nullptr) {
      TempNeighbors.Add(Object);
    }
  }

  int NumRoads = Neighbors.Num() + TempNeighbors.Num();
  
  // if location is occupied set material red for error
  if (ObjectManager->DoesObjectExistHere(Location) || Neighbors.Num() <= 0) {
    return SetNewMaterial(ErrorMaterial);
  }

  
}


// only for use with vehicle markers
// won't work with deadend/straight
FVector ASimpliCityRoadBase::GetVehMarkerNormDirectionVector(UMarkerComponent* VMarker) const {
  if (VMarker == nullptr) {
    TRACE_ERROR_PRINTF(LogSimpliCity, "ERROR!!! VMarker == nullptr");
    return FVector(0, 0, 0);
  }
  FVector SrcLocation;
  FVector DestLocation;
  FVector NormDirectionVector;
  TArray<UMarkerComponent*> AdjacentMarkers;

  // check if this marker has adjacents
  AdjacentMarkers = VMarker->GetAdjacentMarkers();
  if (AdjacentMarkers.Num() > 0) {
    SrcLocation = VMarker->GetComponentLocation();

    if (AdjacentMarkers[0] == nullptr) {
      TRACE_ERROR_PRINTF(LogSimpliCity, "ERROR!!! AdjacentMarkers[0] == nullptr");
      return FVector(0, 0, 0);
    }

    DestLocation = AdjacentMarkers[0]->GetComponentLocation();
  }
  // else, look for this marker as adjacent to another
  else {
    for (auto VehMarker : VehicleMarkers) {
      AdjacentMarkers = VehMarker->GetAdjacentMarkers();
      if (AdjacentMarkers.Contains(VMarker)) {
        SrcLocation = VehMarker->GetComponentLocation();
        DestLocation = VMarker->GetComponentLocation();
        break;
      }
    }
  }

  NormDirectionVector = DestLocation - SrcLocation;
  NormDirectionVector.Normalize();
  return NormDirectionVector;
}

TArray<UMarkerComponent*> ASimpliCityRoadBase::GetClosestMarkerPair(UObject* NeighborRoad, bool IsPedestrian) const {
  TArray<UMarkerComponent*> ClosestMarkers;
  TArray<UMarkerComponent*> CandidateMarkers;
  AMarkerManager* MarkerMgr = USimpliCityFunctionLibrary::GetMarkerManager(this);

  if (IsPedestrian == true) {
    CandidateMarkers = PedestrianMarkers;
  } else {
    CandidateMarkers = VehicleMarkers;
  }

  if (ASimpliCityRoadBase* Road = Cast<ASimpliCityRoadBase>(NeighborRoad)) {
    UMarkerComponent* first = MarkerMgr->GetClosestMarkerTo(Road->GetActorLocation(), CandidateMarkers);
    CandidateMarkers.Remove(first);
    UMarkerComponent* second = MarkerMgr->GetClosestMarkerTo(Road->GetActorLocation(), CandidateMarkers);
    ClosestMarkers.Add(first);
    ClosestMarkers.Add(second);
  }

  if (ClosestMarkers.Num() <= 0) {
    TRACE_ERROR_PRINTF(LogSimpliCity, "ERROR!!! ClosestMarkers.Num() <= 0");
    return TArray<UMarkerComponent*>();
  }
  return ClosestMarkers;
}

TArray<UMarkerComponent*> ASimpliCityRoadBase::GetPedestrianMarkers() const {
  return PedestrianMarkers;
}

TArray<UMarkerComponent*> ASimpliCityRoadBase::GetVehicleMarkers() const {
  return VehicleMarkers;
}

TArray<UObject*> ASimpliCityRoadBase::GetNeighborsOfSameType() const {
  TArray<UObject*> OutNeighbors;
  TArray<ASimpliCityObjectBase*> Neighbors =
      USimpliCityFunctionLibrary::GetObjectManager(this)->GetNeighborsOfType(GetActorLocation(), ObjectType);
  OutNeighbors.Append(Neighbors);
  return OutNeighbors;
}
