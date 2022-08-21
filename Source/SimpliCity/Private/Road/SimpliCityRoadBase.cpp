// Fill out your copyright notice in the Description page of Project Settings.


#include "Road/SimpliCityRoadBase.h"
#include "Components/StaticMeshComponent.h"
#include "MarkerComponent.h"
#include "MarkerManager.h"
#include "Tools/SimpliCityFunctionLibrary.h"

// Sets default values
ASimpliCityRoadBase::ASimpliCityRoadBase()
{
	PrimaryActorTick.bCanEverTick = false;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RoadMesh"));
  SetRootComponent(StaticMeshComponent);
	// TEnumAsByte<ESimpliCityBuildObjectEnum> BuildType;
	// BuildObject_None UMETA(DisplayName = "None"),
	// BuildObject_Road UMETA(DisplayName = "Road")
	BuildType = ESimpliCityBuildObjectEnum::BuildObject_Road;
}

// Called when the game starts or when spawned
void ASimpliCityRoadBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called when the game starts or when spawned
void ASimpliCityRoadBase::SetNewMaterial() {
	check(StaticMeshComponent != nullptr);
	if (RoadMaterial != nullptr) {
		StaticMeshComponent->SetMaterial(0, RoadMaterial);
	}
}

// only for use with vehicle markers
// won't work with deadend/straight
FVector ASimpliCityRoadBase::GetVehMarkerNormDirectionVector(UMarkerComponent* VMarker) {
  check(VMarker != nullptr);
  FVector SrcLocation;
  FVector DestLocation;
  FVector NormDirectionVector;
  TArray<UMarkerComponent*> AdjacentMarkers;

  // check if this marker has adjacents
  AdjacentMarkers = VMarker->GetAdjacentMarkers();
  if (AdjacentMarkers.Num() > 0) {
    SrcLocation = VMarker->GetComponentLocation();
    check(AdjacentMarkers[0] != nullptr);
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

TArray<UMarkerComponent*> ASimpliCityRoadBase::GetClosestMarkerPair(ASimpliCityRoadBase* NeighborRoad, bool IsPedestrian) {
  TArray<UMarkerComponent*> ClosestMarkers;
  TArray<UMarkerComponent*> CandidateMarkers;
  AMarkerManager* MarkerMgr = USimpliCityFunctionLibrary::GetMarkerManager(this);

  if (IsPedestrian == true) {
    CandidateMarkers = PedestrianMarkers;
  }
  else {
    CandidateMarkers = VehicleMarkers;
  }

  UMarkerComponent* first = MarkerMgr->GetClosestMarkerTo(NeighborRoad->GetActorLocation(),CandidateMarkers);

  CandidateMarkers.Remove(first);
  UMarkerComponent* second = MarkerMgr->GetClosestMarkerTo(NeighborRoad->GetActorLocation(),CandidateMarkers);

  ClosestMarkers.Add(first);
  ClosestMarkers.Add(second);
  return ClosestMarkers;
}