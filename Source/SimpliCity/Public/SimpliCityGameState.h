// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "SimpliCityGameState.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SIMPLICITY_API ASimpliCityGameState : public AGameState
{
	GENERATED_BODY()
public:
  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SimpliCityGameState")
  bool CanAfford(ESimpliCityObjectType ObjectType, ESimpliCityResourceType ResourceType, int NumObjects);
};
