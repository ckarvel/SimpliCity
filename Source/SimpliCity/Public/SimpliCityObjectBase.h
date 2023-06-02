// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SimpliCityObjectType.h"
#include "SimpliObjectBase.h"
#include "SimpliCityObjectBase.generated.h"

UCLASS(Blueprintable)
class SIMPLICITY_API ASimpliCityObjectBase : public AActor {
  GENERATED_BODY()

public:
  ASimpliCityObjectBase();
  UFUNCTION(BlueprintPure, Category = "SimpliCityObjectBase")
  FORCEINLINE TEnumAsByte<ESimpliCityObjectType> Type() const {
    return ObjectType;
  }

protected:
  virtual void BeginPlay() override;

public:
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SimpliCityObjectBase")
  UTexture2D* BuildIcon;

  virtual void SetNewMaterial();

  // this object is placed permanently on the grid, start any necessary component behaviors
  virtual void OnObjectPlaced();

protected:
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SimpliCityObjectBase")
  TEnumAsByte<ESimpliCityObjectType> ObjectType;
};
