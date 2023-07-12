// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpliCityObjectType.generated.h"

UENUM(BlueprintType)
enum class ESimpliCityObjectType : uint8 {
  None = 0,
  Road = 1,
  Building = 2,
  Zone = 3
};
