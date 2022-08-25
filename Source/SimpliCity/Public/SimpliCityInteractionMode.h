// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpliCityInteractionMode.generated.h"

UENUM(BlueprintType)
enum ESimpliCityInteractionMode {
  InteractionMode_None UMETA(DisplayName = "None"),
  InteractionMode_Road UMETA(DisplayName = "Road"),
  InteractionMode_Zone UMETA(DisplayName = "Zone"),
  InteractionMode_Building UMETA(DisplayName = "Building")
};