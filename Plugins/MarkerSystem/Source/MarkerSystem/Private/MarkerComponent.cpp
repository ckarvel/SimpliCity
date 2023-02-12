// Property of Carina Karvel

#include "MarkerComponent.h"

UMarkerComponent::UMarkerComponent()
  : openForConnections(true),
    laneDirection(0)
{
  PrimaryComponentTick.bCanEverTick = false;
}

#if WITH_EDITOR
void UMarkerComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {

  FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
  if (PropertyName == GET_MEMBER_NAME_CHECKED(UMarkerComponent,visualizeComponent)) {
    bVisualizeComponent = visualizeComponent;
  }
  Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

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