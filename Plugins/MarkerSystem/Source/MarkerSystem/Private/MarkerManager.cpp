// Property of Carina Karvel

#include "MarkerManager.h"
#include "MarkerComponent.h"
#include "..\Public\MarkerManager.h"

AMarkerManager::AMarkerManager() {
  PrimaryActorTick.bCanEverTick = false;
}

void AMarkerManager::BeginPlay() {
  Super::BeginPlay();
}

TArray<FVector> AMarkerManager::GetNeighbors(FVector Location) const {
  UMarkerComponent* const * marker = LocToMarkerMapping.Find(Location);
  check(marker!= nullptr);
  const TArray<UMarkerComponent*>* connections = MarkerAdjList.Find(*marker);
  check(connections != nullptr);
  TArray<FVector> Locations;
  for (auto connection : *connections) {
    check(connection != nullptr);
    Locations.Add(connection->GetComponentLocation());
  }
  return Locations;
}

UMarkerComponent* AMarkerManager::GetClosestMarkerTo(FVector Location, TArray<UMarkerComponent*> Markers) {
  check(Markers.Num() > 0);
  check(Markers[0] != nullptr);

  UMarkerComponent* closestMarker = Markers[0];
  float minDistance = FVector::Distance(Location,closestMarker->GetComponentLocation());

  for (int i = 1; i < Markers.Num(); i++) {
    check(Markers[i] != nullptr);
    float distance = FVector::Distance(Location,Markers[i]->GetComponentLocation());
    if (distance < minDistance) {
      closestMarker = Markers[i];
      minDistance = distance;
    }
  }
  return closestMarker;
}

void AMarkerManager::AddMarker(UMarkerComponent* Marker) {
  MarkerAdjList.Add(Marker, Marker->GetAdjacentMarkers());
  LocToMarkerMapping.Add(Marker->GetComponentLocation(), Marker);
}

void AMarkerManager::RemoveMarker(UMarkerComponent* Marker) {
  MarkerAdjList.Remove(Marker);
  LocToMarkerMapping.Remove(Marker->GetComponentLocation());
}

void AMarkerManager::AddMarkers(TArray<UMarkerComponent*> Markers) {
  for (auto Marker : Markers) {
    AddMarker(Marker);
  }
}

void AMarkerManager::RemoveMarkers(TArray<UMarkerComponent*> Markers) {
  for (auto Marker : Markers) {
    RemoveMarker(Marker);
  }
}

void AMarkerManager::AddEdge(UMarkerComponent* Src,UMarkerComponent* Dest, bool IsBidirectional) {
  check(Src != nullptr);
  check(Dest != nullptr);
  check(Src != Dest);

  // Find returns a pointer to the value
  TArray<UMarkerComponent*>* pSrc_Connections = MarkerAdjList.Find(Src);
  // if pointer is null key doesn't exist, so add
  if (pSrc_Connections == nullptr)
    AddMarker(Src);
  MarkerAdjList[Src].Add(Dest);
  if (IsBidirectional) {
    TArray<UMarkerComponent*> Dest_Connections = MarkerAdjList.FindOrAdd(Dest);
    Dest_Connections.Add(Src);
  }
}

void AMarkerManager::RemoveEdge(UMarkerComponent* Src, UMarkerComponent* Dest, bool IsBidirectional) {
  check(Src != nullptr);
  check(Dest != nullptr);
  check(Src != Dest);
  TArray<UMarkerComponent*> Src_Connections = MarkerAdjList.FindOrAdd(Src);
  Src_Connections.Remove(Dest);
  if (IsBidirectional) {
    TArray<UMarkerComponent*> Dest_Connections = MarkerAdjList.FindOrAdd(Dest);
    Dest_Connections.Remove(Src);
  }
}
