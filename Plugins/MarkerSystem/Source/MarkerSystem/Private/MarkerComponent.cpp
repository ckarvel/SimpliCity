// Property of Carina Karvel

#include "MarkerComponent.h"

UMarkerComponent::UMarkerComponent()
  : openForConnections(true),
    laneDirection(0)
{
  PrimaryComponentTick.bCanEverTick = false;
}

void UMarkerComponent::BeginPlay() {
  Super::BeginPlay();
}

void UMarkerComponent::AddAdjacentMarker(UMarkerComponent* Marker) {
  if (Marker == nullptr)
    return;
  AdjacentMarkers.Add(Marker);
}

TArray<FVector> UMarkerComponent::GetAdjacentMarkerLocations() {
  TArray<FVector> Markers;
  for (auto& Marker : AdjacentMarkers)
    Markers.Add(Marker->GetComponentLocation());
  return Markers;
}

bool UMarkerComponent::IsSourceMarker() {
  if (AdjacentMarkers.Num() > 0) {
    return false;
  }
  return true;
}