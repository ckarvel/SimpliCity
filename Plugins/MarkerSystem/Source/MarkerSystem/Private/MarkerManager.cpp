// Property of Carina Karvel

#include "MarkerManager.h"
#include "MarkerComponent.h"
#include "MarkerPathInterface.h"

AMarkerManager::AMarkerManager() {
  PrimaryActorTick.bCanEverTick = false;
}

void AMarkerManager::BeginPlay() {
  Super::BeginPlay();
}

TArray<FVector> AMarkerManager::GetNeighbors(FVector Location) const {
  UMarkerComponent* const * marker = LocToMarkerMapping.Find(Location);
  check(marker!= nullptr);
  const TArray<UMarkerComponent*>* connections = MarkerConnList.Find(*marker);
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
  if (MarkerConnList.Contains(Marker) == true) 
    return; // already exists

  MarkerConnList.Add(Marker, Marker->GetAdjacentMarkers());
  LocToMarkerMapping.Add(Marker->GetComponentLocation(),Marker);
}

void AMarkerManager::RemoveMarker(UMarkerComponent* Marker) {
  MarkerConnList.Remove(Marker);
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
  TArray<UMarkerComponent*>* pSrc_Connections = MarkerConnList.Find(Src);
  // if pointer is null key doesn't exist, so add
  if (pSrc_Connections == nullptr)
    AddMarker(Src);
  MarkerConnList[Src].Add(Dest);
  if (IsBidirectional) {
    TArray<UMarkerComponent*>* Dest_Connections = MarkerConnList.Find(Dest);
    // if pointer is null key doesn't exist, so add
    if (Dest_Connections == nullptr)
      AddMarker(Dest);
    MarkerConnList[Dest].Add(Src);
  }
}

void AMarkerManager::RemoveEdge(UMarkerComponent* Src, UMarkerComponent* Dest, bool IsBidirectional) {
  check(Src != nullptr);
  check(Dest != nullptr);
  check(Src != Dest);
  TArray<UMarkerComponent*> Src_Connections = MarkerConnList.FindOrAdd(Src);
  Src_Connections.Remove(Dest);
  if (IsBidirectional) {
    TArray<UMarkerComponent*> Dest_Connections = MarkerConnList.FindOrAdd(Dest);
    Dest_Connections.Remove(Src);
  }
}

void AMarkerManager::ClearGraph() {
  MarkerConnList.Empty();
  LocToMarkerMapping.Empty();
}

void AMarkerManager::GetConnectionsFrom(UMarkerComponent* InMarker,TArray<UMarkerComponent*>& OutConnections) {
  TArray<UMarkerComponent*>* pSrc_Connections = MarkerConnList.Find(InMarker);
  if (pSrc_Connections != nullptr) {
    OutConnections = *pSrc_Connections;
  }
}

void AMarkerManager::UpdateGraph(const TArray<UObject*>& PathObjects) {
  AddMarkersToGraph(PathObjects);
  for (UObject* Object : PathObjects) {
    IMarkerPathInterface* MarkerPathObject = Cast<IMarkerPathInterface>(Object);
    TArray<UObject*> Neighbors = MarkerPathObject->GetNeighborsOfSameType();
    for (UObject* Neighbor : Neighbors) {
      AddNeighborConnections(Object, Neighbor, true);
      AddNeighborConnections(Object,Neighbor, false);
    }
  }
}

void AMarkerManager::AddMarkersToGraph(const TArray<UObject*>& PathObjects) {
  ClearGraph();
  for (UObject* Object : PathObjects) {
    check(Object->Implements<UMarkerPathInterface>() == true);
    IMarkerPathInterface* MarkerPathObject = Cast<IMarkerPathInterface>(Object);
    AddMarkers(MarkerPathObject->GetPedestrianMarkers());
    AddMarkers(MarkerPathObject->GetVehicleMarkers());
  }
}

void AMarkerManager::AddNeighborConnections(UObject* ObjectA, UObject* ObjectB, bool isPedestrian) {
  TArray<UMarkerComponent*> AMarkers;
  TArray<UMarkerComponent*> BMarkers;
  // get closest markers for each road object
  IMarkerPathInterface* MarkerPathObjectA = Cast<IMarkerPathInterface>(ObjectA);
  AMarkers = MarkerPathObjectA->GetClosestMarkerPair(ObjectB,isPedestrian);
  IMarkerPathInterface* MarkerPathObjectB = Cast<IMarkerPathInterface>(ObjectB);
  BMarkers = MarkerPathObjectB->GetClosestMarkerPair(ObjectA,isPedestrian);
  
  for (auto MarkerA : AMarkers) {
    UMarkerComponent* MarkerB = GetClosestMarkerTo(MarkerA->GetComponentLocation(), BMarkers);
    // verify lane dirs are equal
    if (MarkerA->laneDirection != MarkerB->laneDirection) {
      FVector AVect = MarkerPathObjectA->GetVehMarkerNormDirectionVector(MarkerA);
      FVector BVect = MarkerPathObjectB->GetVehMarkerNormDirectionVector(MarkerB);
      check(FMath::IsNearlyEqual(AVect.X,BVect.X,25) && FMath::IsNearlyEqual(AVect.Y,BVect.Y,25));
    }
    if (MarkerA->IsSourceMarker()) {
      AddEdge(MarkerA, MarkerB, isPedestrian);
    }
    else {
      AddEdge(MarkerB,MarkerA,isPedestrian);
    }
  }
}