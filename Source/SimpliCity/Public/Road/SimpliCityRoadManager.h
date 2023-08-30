// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PathFinderInterface.h"
#include "SimpliCityBaseManager.h"
#include "SimpliCityRoadManager.generated.h"

class AMarkerManager;
class ASimpliCityObjectBase;
class ASimpliCityRoadBase;
class USimpliCityRoadFixerComponent;

UCLASS(Blueprintable)
class SIMPLICITY_API ASimpliCityRoadManager : public ASimpliCityBaseManager, public IPathFinderInterface {
  GENERATED_BODY()

public:
  // Sets default values for this actor's properties
  ASimpliCityRoadManager();

protected:
  UFUNCTION(BlueprintCallable, Category = "SimpliCityRoadManager")
  virtual void StartBuilding() override;
  UFUNCTION(BlueprintCallable, Category = "SimpliCityRoadManager")
  virtual bool UpdateBuilding(FVector Location) override;
  UFUNCTION(BlueprintCallable, Category = "SimpliCityRoadManager")
  virtual void FinishBuilding() override;
  UFUNCTION(BlueprintCallable, Category = "SimpliCityRoadManager")
  virtual void CancelBuilding() override;
  UFUNCTION(BlueprintCallable, Category = "SimpliCityRoadManager")
  virtual void DestroyObjects(const TArray<ASimpliCityObjectBase*>& ObjectList) override;

private:
  void CreateTemporaryRoadsAtLocations(const TArray<FVector>& Locations);
  void ConvertAllTemporaryToPermanent();
  void DestroyTemporaryRoadsAtLocations(const TArray<FVector> Locations);
  void FixRoad(ASimpliCityObjectBase* Road);
  void FixNeighborsAtLocation(FVector Location);
  void FixRoadAndNeighbors(ASimpliCityObjectBase* Road);
  TArray<ASimpliCityObjectBase*> GetTempAndPermNeighbors(FVector Location) const;

public:
  virtual void Enable(ESimpliCityResourceType _TypeId) override;
  virtual void Disable() override;

  UFUNCTION(BlueprintCallable, Category = "SimpliCityRoadManager")
  virtual TArray<FVector> GetNeighbors(FVector Location) const override;
  virtual float GetCost(FVector Location) const override {
    return 1.0;
  } // todo

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SimpliCityRoadManager")
  USimpliCityRoadFixerComponent* RoadFixerComponent;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpliCityRoadManager")
  AMarkerManager* AgentMarkerGraph;

private:
  TSet<FVector> oldPath;
};
