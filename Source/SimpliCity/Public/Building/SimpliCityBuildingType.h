// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpliCityBuildingType.generated.h"

UENUM(BlueprintType)
enum class ESimpliCityBuildingType : uint8 {
  None = 0,
  Factory = 1,
  Farm = 2,
  Zoned = 3
};
