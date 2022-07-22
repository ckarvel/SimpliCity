// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SimpliCityFunctionLibrary.generated.h"

class AGridManager;
class ASimpliCityPlayerController;
/**
 *
 */
UCLASS()
class SIMPLICITY_API USimpliCityFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
  UFUNCTION(BlueprintPure,Category="SimpliLib", meta=(WorldContext="WorldContextObject", UnsafeDuringActorConstruction="true"))
	static AGridManager* GetGridManager(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure,Category="SimpliLib",meta=(WorldContext="WorldContextObject",UnsafeDuringActorConstruction="true"))
	static ASimpliCityPlayerController* GetPlayerController(const UObject* WorldContextObject);
};
