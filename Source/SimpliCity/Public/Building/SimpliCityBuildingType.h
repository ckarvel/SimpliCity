// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpliCityBuildingType.generated.h"

UENUM(BlueprintType)
enum ESimpliCityBuildingType {
  BuildingType_None UMETA(DisplayName = "None"),
  BuildingType_Power UMETA(DisplayName = "Power"),
  BuildingType_Water UMETA(DisplayName = "Water"),
  BuildingType_Zoned UMETA(DisplayName = "Zoned")
};
