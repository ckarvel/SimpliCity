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
  UFUNCTION(BlueprintPure,Category="SimpliCityFunctionLibrary", meta=(WorldContext="WorldContextObject", UnsafeDuringActorConstruction="true"))
	static class AGridManager* GetGridManager(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure,Category="SimpliCityFunctionLibrary",meta=(WorldContext="WorldContextObject",UnsafeDuringActorConstruction="true"))
	static class ASimpliCityPlayerController* GetPlayerController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure,Category="SimpliCityFunctionLibrary",meta=(WorldContext="WorldContextObject",UnsafeDuringActorConstruction="true"))
	static class ASimpliCityBuildManager* GetBuildManager(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure,Category="SimpliCityFunctionLibrary",meta=(WorldContext="WorldContextObject",UnsafeDuringActorConstruction="true"))
	static class AMarkerManager* GetMarkerManager(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure,Category="SimpliCityFunctionLibrary",meta=(WorldContext="WorldContextObject",UnsafeDuringActorConstruction="true"))
	static class ASimpliCityRoadManager* GetRoadManager(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure,Category="SimpliCityFunctionLibrary")
	static bool AreLocationsEqual(FVector LocationA, FVector LocationB, float Tolerance=25);

	UFUNCTION(BlueprintPure,Category="SimpliCityFunctionLibrary")
	static TArray<FVector> GetPathBetween(UObject* Graph,FVector Start,FVector End);

	UFUNCTION(BlueprintPure,Category="SimpliCityFunctionLibrary")
	static FVector VInterpTo(FVector Current,FVector Target,float Delta,float Speed);

	UFUNCTION(BlueprintPure,Category="SimpliCityFunctionLibrary")
	static FRotator RInterpTo(FRotator Current,FRotator Target,float Delta,float Speed);

	UFUNCTION(BlueprintPure,Category="SimpliCityFunctionLibrary")
	static float FInterpTo(float Current,float Target,float Delta,float Speed);

	UFUNCTION(BlueprintPure,Category="SimpliCityFunctionLibrary")
	static bool IsNearlyEqual(float A, float B);
};