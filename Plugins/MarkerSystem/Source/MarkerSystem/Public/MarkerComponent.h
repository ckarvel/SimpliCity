// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "MarkerComponent.generated.h"


UCLASS(Blueprintable,ClassGroup=(Custom),meta=(BlueprintSpawnableComponent))
class MARKERSYSTEM_API UMarkerComponent : public USceneComponent {
  GENERATED_BODY()

public:
  UMarkerComponent();

protected:
  virtual void BeginPlay() override;

public:
  UFUNCTION(BlueprintCallable,Category = "Marker")
  void AddAdjacentMarker(UMarkerComponent* Marker);

  UFUNCTION(BlueprintCallable,Category = "Marker")
  TArray<FVector> GetAdjacentMarkerLocations();

  UFUNCTION(BlueprintCallable,Category = "Marker")
  TArray<UMarkerComponent*> GetAdjacentMarkers() { return AdjacentMarkers; }

  UFUNCTION(BlueprintCallable,Category = "Marker")
  bool IsSourceMarker();

  UPROPERTY(EditAnywhere,BlueprintReadWrite)
  bool openForConnections;

  // 1 is Right lane
  // 0 can be right or left (turns/sidewalks)
  // -1 is Left lane
  UPROPERTY(EditAnywhere,BlueprintReadWrite)
  int laneDirection;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
  TArray<UMarkerComponent*> AdjacentMarkers;

  UFUNCTION(BlueprintCallable,Category = "Marker")
  bool IsOpen() const { return openForConnections; }
};
