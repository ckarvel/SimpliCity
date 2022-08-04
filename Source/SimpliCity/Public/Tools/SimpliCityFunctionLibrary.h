// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SimpliCityFunctionLibrary.generated.h"

/**
 *
 */

UCLASS(Blueprintable)
class SIMPLICITY_API USimpliCityFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	USimpliCityFunctionLibrary() {};

	// getters
  UFUNCTION(BlueprintPure,Category="SimpliLib", meta=(WorldContext="WorldContextObject", UnsafeDuringActorConstruction="true"))
	static class AGridManager* GetGridManager(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure,Category="SimpliLib",meta=(WorldContext="WorldContextObject",UnsafeDuringActorConstruction="true"))
	static class ASimpliCityPlayerController* GetPlayerController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure,Category="SimpliLib",meta=(WorldContext="WorldContextObject",UnsafeDuringActorConstruction="true"))
	static class ASimpliCityBuildManager* GetBuildManager(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure,Category="SimpliLib",meta=(WorldContext="WorldContextObject",UnsafeDuringActorConstruction="true"))
	static class AMarkerManager* GetMarkerManager(const UObject* WorldContextObject);
	
	// tools
	UFUNCTION(BlueprintPure,Category="SimpliLib")
	static bool AreLocationsEqual(FVector LocationA, FVector LocationB);
};
