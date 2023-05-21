// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "SimpliCity.h"
#include "SimpliCityFunctionLibrary.generated.h"

/**
 *
 */

UCLASS(Blueprintable)
class SIMPLICITY_API USimpliCityFunctionLibrary : public UBlueprintFunctionLibrary {
  GENERATED_BODY()
public:
  USimpliCityFunctionLibrary() {};

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Getters
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  UFUNCTION(BlueprintPure, Category = "SimpliCityFunctionLibrary",
    meta = (WorldContext = "WorldContextObject", UnsafeDuringActorConstruction = "true"))
  static class AGridManager* GetGridManager(const UObject* WorldContextObject);

  UFUNCTION(BlueprintPure, Category = "SimpliCityFunctionLibrary",
    meta = (WorldContext = "WorldContextObject", UnsafeDuringActorConstruction = "true"))
  static class ASimpliCityPlayerController* GetPlayerController(const UObject* WorldContextObject);

  UFUNCTION(BlueprintPure, Category = "SimpliCityFunctionLibrary",
    meta = (WorldContext = "WorldContextObject", UnsafeDuringActorConstruction = "true"))
  static class USimpliCityMainUI* GetMainUI(UObject* WorldContextObject);

  UFUNCTION(BlueprintPure, Category = "SimpliCityFunctionLibrary",
    meta = (WorldContext = "WorldContextObject", UnsafeDuringActorConstruction = "true"))
  static class ASimpliCityObjectManager* GetObjectManager(const UObject* WorldContextObject);

  UFUNCTION(BlueprintPure, Category = "SimpliCityFunctionLibrary",
    meta = (WorldContext = "WorldContextObject", UnsafeDuringActorConstruction = "true"))
  static class AMarkerManager* GetMarkerManager(const UObject* WorldContextObject);

  UFUNCTION(BlueprintPure, Category = "SimpliCityFunctionLibrary",
    meta = (WorldContext = "WorldContextObject", UnsafeDuringActorConstruction = "true"))
  static class ASimpliCityRoadManager* GetRoadManager(const UObject* WorldContextObject);

  UFUNCTION(BlueprintPure, Category = "SimpliCityFunctionLibrary",
    meta = (WorldContext = "WorldContextObject", UnsafeDuringActorConstruction = "true"))
  static class ASimpliCityZoneManager* GetZoneManager(const UObject* WorldContextObject);

  UFUNCTION(BlueprintPure, Category = "SimpliCityFunctionLibrary",
    meta = (WorldContext = "WorldContextObject", UnsafeDuringActorConstruction = "true"))
  static class ASimpliCityBuildingManager* GetBuildingManager(const UObject* WorldContextObject);

  template <class ManagerClass>
  static ManagerClass* GetManager(const UObject* WorldContextObject, TSubclassOf<ManagerClass> Class);

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Common Math Functions
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  UFUNCTION(BlueprintPure, Category = "SimpliCityFunctionLibrary")
  static bool IsNearlyEqual(float A, float B);

  UFUNCTION(BlueprintPure, Category = "SimpliCityFunctionLibrary")
  static bool AreLocationsEqual(FVector LocationA, FVector LocationB, float Tolerance = 25);

  UFUNCTION(BlueprintPure, Category = "SimpliCityFunctionLibrary")
  static FVector GetMidpointBetween(FVector A, FVector B);

  UFUNCTION(BlueprintPure, Category = "SimpliCityFunctionLibrary")
  static FVector VInterpTo(FVector Current, FVector Target, float Delta, float Speed);

  UFUNCTION(BlueprintPure, Category = "SimpliCityFunctionLibrary")
  static FRotator RInterpTo(FRotator Current, FRotator Target, float Delta, float Speed);

  UFUNCTION(BlueprintPure, Category = "SimpliCityFunctionLibrary")
  static float FInterpTo(float Current, float Target, float Delta, float Speed);

  UFUNCTION(BlueprintPure, Category = "SimpliCityFunctionLibrary")
  static bool ArePointsCollinear(FVector A, FVector B, FVector C);

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Math Algorithms
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  UFUNCTION(BlueprintCallable, Category = "SimpliCityFunctionLibrary")
  static TArray<AActor*> GetDifferenceInArrays(TArray<AActor*> A, TArray<AActor*> B);

  UFUNCTION(BlueprintCallable, Category = "SimpliCityFunctionLibrary")
  static void CalculateSelectionRectangle(
    FVector Start, FVector End, FVector& OutExtents, TArray<FVector>& OutVertices, TArray<int>& OutTriangles);

  UFUNCTION(BlueprintCallable, Category = "SimpliCityFunctionLibrary")
  static TArray<FVector> GetPathBetween(UObject* Graph, FVector Start, FVector End);

  UFUNCTION(BlueprintCallable, Category = "SimpliCityFunctionLibrary")
  static TArray<FVector> GetSimplifiedPath(TArray<FVector> Path);

  UFUNCTION(BlueprintCallable, Category = "SimpliCityFunctionLibrary")
  static float AngleBetween2Vectors(FVector A, FVector B);

  UFUNCTION(BlueprintCallable, Category = "SimpliCityFunctionLibrary")
  static TArray<FVector> SmoothCurvedSegments(TArray<FVector> Path);

  UFUNCTION(BlueprintCallable, Category = "SimpliCityFunctionLibrary")
  static TArray<FVector> QuadraticBezierCurve(FVector P0, FVector P1, FVector P2);
};
