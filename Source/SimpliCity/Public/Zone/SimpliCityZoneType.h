// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpliCityZoneType.generated.h"

UENUM(BlueprintType)
enum ESimpliCityZoneType {
  ZoneType_None UMETA(DisplayName = "None"),
  ZoneType_Residential UMETA(DisplayName = "Residential"),
  ZoneType_Commercial UMETA(DisplayName = "Commercial"),
  ZoneType_Industrial UMETA(DisplayName = "Industrial")
};
