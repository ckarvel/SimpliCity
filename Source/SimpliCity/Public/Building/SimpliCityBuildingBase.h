// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SimpliCityBuildingType.h"
#include "SimpliCityObjectBase.h"
#include "SimpliCityBuildingBase.generated.h"

UCLASS(Blueprintable)
class SIMPLICITY_API ASimpliCityBuildingBase : public ASimpliCityObjectBase {
  GENERATED_BODY()

protected:
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpliCityBuildingBase")
  class ASimpliCityRoadBase* RoadConnection;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpliCityBuildingBase", Meta = (ExposeOnSpawn = true))
  ESimpliCityBuildingType BuildingType;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpliCityBuildingBase")
  float YawOffset;

public:
  ASimpliCityBuildingBase();

  virtual void SetNewLocation(FVector Location) override;

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SimpliCityBuildingBase")
  void OnNewLocation(bool IsValid);

  UFUNCTION(BlueprintCallable, Category = "SimpliCityBuildingBase")
  ASimpliCityRoadBase *GetRoadConnection() const { return RoadConnection; }

  UFUNCTION(BlueprintCallable, Category = "SimpliCityBuildingBase")
  ESimpliCityBuildingType GetBuildingType() const { return BuildingType; }
};
