// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SimpliObjectBase.generated.h"

class ASimpliCityObjectBase;
class ASimpliCityRoadBase;
/**
 *
 */
UCLASS()
class SIMPLICITY_API USimpliObjectBase : public UObject {
  GENERATED_BODY()
public:
  USimpliObjectBase() {
  }
  ASimpliCityObjectBase* SpawnActor(FVector Location);
  void DestroyActor();

protected:
  // Default class to spawn if none given
  TSubclassOf<ASimpliCityObjectBase> DefaultClass;
  // Actual class of the spawned object
  TSubclassOf<ASimpliCityObjectBase> ActorClass;
  // The actor owned by this class and may or may not physically exist in the World
  ASimpliCityObjectBase* SpawnedActor;
};

UCLASS()
class SIMPLICITY_API USimpliRoadBase : public USimpliObjectBase {
  GENERATED_BODY()
protected:
  TSubclassOf<ASimpliCityRoadBase> Road_4WayClass;
  TSubclassOf<ASimpliCityRoadBase> Road_3WayClass;
  TSubclassOf<ASimpliCityRoadBase> Road_2Way_CornerClass;
  TSubclassOf<ASimpliCityRoadBase> Road_2Way_StraightClass;
  TSubclassOf<ASimpliCityRoadBase> Road_1WayClass;
  ASimpliCityRoadBase* NeighboringObjects;
};

UCLASS()
class SIMPLICITY_API USimpliBuildingBase : public USimpliObjectBase {
  GENERATED_BODY()
protected:
  ASimpliCityRoadBase* RoadConnection;
};

UCLASS()
class SIMPLICITY_API USimpliZoneBase : public USimpliObjectBase {
  GENERATED_BODY()
protected:
  ASimpliCityRoadBase* RoadConnection;
};
