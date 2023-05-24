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
  virtual void SetNewMaterial();

  // this object is placed permanently on the grid, start any necessary component behaviors
  virtual void OnObjectPlaced();

  void SetObjectBase(USimpliObjectBase* Base) {
    ObjectBase = Base;
  }

  USimpliObjectBase* GetObjectBase() {
    return ObjectBase;
  }

protected:
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SimpliCityObjectBase")
  TEnumAsByte<ESimpliCityObjectType> ObjectType;

  USimpliObjectBase* ObjectBase;
};
