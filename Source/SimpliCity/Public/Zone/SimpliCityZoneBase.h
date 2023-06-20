// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SimpliCityObjectBase.h"
#include "SimpliCityZoneType.h"
#include "SimpliCityZoneBase.generated.h"

UCLASS()
class SIMPLICITY_API ASimpliCityZoneBase : public ASimpliCityObjectBase {
  GENERATED_BODY()

public:
  // Sets default values for this actor's properties
  ASimpliCityZoneBase();

  UFUNCTION(BlueprintCallable, Category = "SimpliCityZoneBase")
  TEnumAsByte<ESimpliCityZoneType> GetZoneType() const {
    return ZoneType;
  }

  UFUNCTION(BlueprintCallable, Category = "SimpliCityZoneBase")
  void SetZoneType(TEnumAsByte<ESimpliCityZoneType> Type, UMaterialInstance* Material);

  UFUNCTION(BlueprintCallable, Category = "SimpliCityZoneBase")
  void ShowActiveMaterial();

protected:
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpliCityZoneBase", Meta = (ExposeOnSpawn = true))
  TEnumAsByte<ESimpliCityZoneType> ZoneType;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SimpliCityZoneBase")
  class UMaterialInstance* ActiveMaterial;
};
