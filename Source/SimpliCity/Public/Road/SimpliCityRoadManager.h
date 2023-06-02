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
  virtual void Update(FVector Location) override;
  // UFUNCTION(BlueprintCallable, Category = "SimpliCityRoadManager")
  // virtual void Update_Implementation(FVector Location) override;
  UFUNCTION(BlueprintCallable, Category = "SimpliCityRoadManager")
  virtual void StartBuilding(FVector Location) override;
  UFUNCTION(BlueprintCallable, Category = "SimpliCityRoadManager")
  virtual void FinishBuilding() override;
  UFUNCTION(BlueprintCallable, Category = "SimpliCityRoadManager")
  virtual void CancelBuilding() override;
  UFUNCTION(BlueprintCallable, Category = "SimpliCityRoadManager")
  virtual void DestroyObjects(const TArray<ASimpliCityObjectBase*>& ObjectList) override;

  UFUNCTION(BlueprintCallable, Category = "SimpliCityRoadManager")
  bool PlacePermanentRoad(const FVector Location, const FRotator Rotation);
  UFUNCTION(BlueprintCallable, Category = "SimpliCityRoadManager")
  void CreateTemporaryRoadsAtLocations(const TArray<FVector>& Locations);

  void DestroyTemporaryRoadsAtLocations(const TArray<FVector> Locations);
  void DestroyAllTemporaryRoads();

  UFUNCTION(BlueprintCallable, Category = "SimpliCityRoadManager")
  void ConvertAllTemporaryToPermanent();
  //////////////////////////////////////////////////////////////////////////
  // destroy functions
  UFUNCTION(BlueprintCallable, Category = "SimpliCityRoadManager")
  void DestroyPermanentRoad(ASimpliCityObjectBase* Road);

public:
  UFUNCTION(BlueprintCallable, Category = "SimpliCityRoadManager")
  virtual void Enable(UTexture2D* NewIcon) override;
  UFUNCTION(BlueprintCallable, Category = "SimpliCityRoadManager")
  virtual void Disable() override;

  void FixRoad(ASimpliCityObjectBase* Road);
  void SwapRoads(ASimpliCityObjectBase* OldRoad, ASimpliCityObjectBase* NewRoad);

  UFUNCTION(BlueprintCallable, Category = "SimpliCityRoadManager")
  virtual TArray<FVector> GetNeighbors(FVector Location) const override;

  UFUNCTION(BlueprintCallable, Category = "SimpliCityRoadManager")
  virtual float GetCost(FVector Location) const override {
    return 1.0;
  } // todo

  UFUNCTION(BlueprintCallable, Category = "SimpliCityRoadManager")
  void FixNeighborsAtLocation(FVector Location);

  UFUNCTION(BlueprintCallable, Category = "SimpliCityRoadManager")
  void FixRoadAndNeighbors(ASimpliCityObjectBase* Road);

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SimpliCityRoadManager")
  USimpliCityRoadFixerComponent* RoadFixerComponent;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpliCityRoadManager")
  AMarkerManager* AgentMarkerGraph;

private:
  TSet<FVector> oldPath;
};
