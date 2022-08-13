// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PathFinderInterface.h"
#include "MarkerManager.generated.h"

// we will have two marker graph objects. 1 for pedestrian, 1 for vehicle
UCLASS(Blueprintable)
class MARKERSYSTEM_API AMarkerManager : public AActor, public IPathFinderInterface {
  GENERATED_BODY()

public:
  AMarkerManager();

protected:
  virtual void BeginPlay() override;

  // this is the marker graph used in pathing
  // can't be used in blueprint because nested container not supported
  TMap<class UMarkerComponent*,TArray<UMarkerComponent*>> MarkerConnList;

  // for easy finding of markers
  UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = "Marker")
  TMap<FVector,UMarkerComponent*> LocToMarkerMapping;
 
public:
  UFUNCTION(BlueprintCallable,Category = "Marker")
  virtual TArray<FVector> GetNeighbors(FVector Location) const override;

  UFUNCTION(BlueprintCallable,Category = "Marker")
  virtual float GetCost(FVector Location) const override { return 1.0; } // todo

  UFUNCTION(BlueprintCallable,Category = "Marker")
  UMarkerComponent* GetClosestMarkerTo(FVector Location, TArray<UMarkerComponent*> Markers);

  UFUNCTION(BlueprintCallable,Category = "Marker")
  void AddMarker(UMarkerComponent* Marker);

  UFUNCTION(BlueprintCallable,Category = "Marker")
  void RemoveMarker(UMarkerComponent* Marker);

  UFUNCTION(BlueprintCallable,Category = "Marker")
  void AddMarkers(TArray<UMarkerComponent*> Markers);

  UFUNCTION(BlueprintCallable,Category = "Marker")
  void RemoveMarkers(TArray<UMarkerComponent*> Markers);

  // source to destination
  // a to b
  UFUNCTION(BlueprintCallable,Category = "Marker")
  void AddEdge(UMarkerComponent* Src, UMarkerComponent* Dest, bool IsBidirectional = true);

  UFUNCTION(BlueprintCallable,Category = "Marker")
  void RemoveEdge(UMarkerComponent* MarkerA,UMarkerComponent* MarkerB, bool IsBidirectional = true);

  UFUNCTION(BlueprintCallable,Category = "Marker")
  void GetConnectionsFrom(UMarkerComponent* InMarker,TArray<UMarkerComponent*>& OutConnections);

protected:
  UFUNCTION(BlueprintCallable,Category = "Marker")
  void ClearGraph();
};
