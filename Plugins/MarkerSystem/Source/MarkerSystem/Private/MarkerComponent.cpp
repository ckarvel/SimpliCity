// Property of Carina Karvel

#include "MarkerComponent.h"

UMarkerComponent::UMarkerComponent()
  : openForConnections(true)
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
