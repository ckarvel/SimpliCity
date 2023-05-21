// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PathFinderInterface.generated.h"
/**
 *
 */
UINTERFACE(minimalapi, meta = (CannotImplementInterfaceInBlueprint))
class UPathFinderInterface : public UInterface {
  GENERATED_BODY()
};

class IPathFinderInterface {
  GENERATED_BODY()

public:
  virtual TArray<FVector> GetNeighbors(FVector) const = 0;
  virtual float GetCost(FVector) const = 0;
  // UFUNCTION(BlueprintCallable)
  // virtual void ExposedVirtualFunction() {}
};
