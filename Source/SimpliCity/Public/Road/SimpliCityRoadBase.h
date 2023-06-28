// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MarkerPathInterface.h"
#include "SimpliCity.h"
#include "SimpliCityObjectBase.h"
#include "SimpliCityRoadBase.generated.h"

class UMarkerComponent;
class UStaticMeshComponent;

UCLASS(Blueprintable)
class SIMPLICITY_API ASimpliCityRoadBase : public ASimpliCityObjectBase, public IMarkerPathInterface {
  GENERATED_BODY()

public:
  ASimpliCityRoadBase();

  ///////////////////////////////////// UMarkerPathInterface /////////////////////////////////////
  UFUNCTION(BlueprintCallable, Category = "SimpliCityRoadBase")
  TArray<UMarkerComponent*> GetClosestMarkerPair(UObject* NeighborRoad, bool IsPedestrian) const override;

  UFUNCTION(BlueprintCallable, Category = "SimpliCityRoadBase")
  TArray<UMarkerComponent*> GetPedestrianMarkers() const override;

  UFUNCTION(BlueprintCallable, Category = "SimpliCityRoadBase")
  TArray<UMarkerComponent*> GetVehicleMarkers() const override;

  UFUNCTION(BlueprintCallable, Category = "SimpliCityRoadBase")
  TArray<UObject*> GetNeighborsOfSameType() const override;

  UFUNCTION(BlueprintCallable, Category = "SimpliCityRoadBase")
  FVector GetVehMarkerNormDirectionVector(UMarkerComponent* VMarker) const override;
  ///////////////////////////////////// UMarkerPathInterface /////////////////////////////////////

protected:
  virtual void BeginPlay() override;

  friend class ASimpliCityRoadManager;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpliCityRoadBase")
  class UMaterialInstance* RoadMaterial;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SimpliCityRoadBase")
  TArray<class UMarkerComponent*> PedestrianMarkers;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SimpliCityRoadBase")
  TArray<class UMarkerComponent*> VehicleMarkers;

  TArray<ASimpliCityRoadBase*> NeighboringRoads;

private:
  ASimpliCityRoadManager* RoadManager;
};
