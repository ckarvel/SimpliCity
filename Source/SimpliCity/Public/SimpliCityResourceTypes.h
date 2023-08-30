// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpliCityResourceTypes.generated.h"

UENUM(BlueprintType)
enum class ESimpliCityResourceType : uint8 {
  None = 0,
  Bread = 1,
  Power = 2,
  Water = 3,
  Wheat = 4,
  TwoWayPaved = 5,
  TwoWayDirt = 6,
  Residential = 7,
  Commercial = 8,
  Industrial = 9,
  Money = 10,
};

struct FSimpliCityRoadResource {
  static TArray<ESimpliCityResourceType> Types;
  static bool TryFindResource(ESimpliCityResourceType Type) {
    return Types.Contains(Type);
  }
};

struct FSimpliCityZoneResource {
  static TArray<ESimpliCityResourceType> Types;
  static bool TryFindResource(ESimpliCityResourceType Type) {
    return Types.Contains(Type);
  }
};

struct FSimpliCityFactoryResource {
  static TArray<ESimpliCityResourceType> Types;
  static bool TryFindResource(ESimpliCityResourceType Type) {
    return Types.Contains(Type);
  }
};

struct FSimpliCityFarmResource {
  static TArray<ESimpliCityResourceType> Types;
  static bool TryFindResource(ESimpliCityResourceType Type) {
    return Types.Contains(Type);
  }
};