// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpliCityResourceTypes.generated.h"

UENUM(BlueprintType)
enum class ESimpliCityResourceType : uint8 {
  None = 0,
  Residential = 1,
  Commercial = 2,
  Industrial = 3,
  Money = 4,
  Water = 5,
  Power = 6,
  TwoWay_Asphalt = 7
};
