// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpliCityBuildObjectEnum.generated.h"

UENUM(BlueprintType)
enum ESimpliCityBuildObjectEnum {
  BuildObject_None UMETA(DisplayName = "None"),
  BuildObject_Road UMETA(DisplayName = "Road")
};