// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MarkerPathInterface.generated.h"
/**
 *
 */
class UMarkerComponent;

UINTERFACE(minimalapi,meta=(CannotImplementInterfaceInBlueprint))
class UMarkerPathInterface : public UInterface {
  GENERATED_BODY()
};

class IMarkerPathInterface {
  GENERATED_BODY()

public:
  
  virtual TArray<UMarkerComponent*> GetClosestMarkerPair(UObject* PathObject, bool IsPedestrian) const = 0;
  virtual TArray<UMarkerComponent*> GetPedestrianMarkers() const = 0;
  virtual TArray<UMarkerComponent*> GetVehicleMarkers() const = 0;
  virtual TArray<UObject*> GetNeighborsOfSameType() const = 0;
  virtual FVector GetVehMarkerNormDirectionVector(UMarkerComponent* VMarker) const = 0;
};