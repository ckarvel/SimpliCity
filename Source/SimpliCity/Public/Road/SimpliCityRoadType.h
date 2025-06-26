// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpliCityRoadType.generated.h"

UENUM(BlueprintType)
enum class ESimpliCityRoadType : uint8 {
  None = 0,
  Deadend = 1,
  Straight = 2,
  Corner = 3,
  ThreeWay = 4,
  FourWay = 5
};
