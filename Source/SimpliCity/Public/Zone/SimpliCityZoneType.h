// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpliCityZoneType.generated.h"

UENUM(BlueprintType)
enum class ESimpliCityZoneType : uint8 {
  None = 0,
  Residential= 1,
  Commercial= 2,
  Industrial= 3
};
