// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/SimpliCityFunctionLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GridManager.h"
#include "MarkerManager.h"

#include "SimpliCityGameInstance.h"
#include "Builder/SimpliCityBuildManager.h"
#include "Road/SimpliCityRoadManager.h"
#include "SimpliCityPlayerController.h"

AGridManager* USimpliCityFunctionLibrary::GetGridManager(const UObject* WorldContextObject) {
  USimpliCityGameInstance* gameInst = Cast<USimpliCityGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
  check(gameInst != nullptr)
  return gameInst->GetGridManager();
}

ASimpliCityPlayerController* USimpliCityFunctionLibrary::GetPlayerController(const UObject* WorldContextObject) {
  return Cast<ASimpliCityPlayerController>(UGameplayStatics::GetPlayerController(WorldContextObject, 0));
}

ASimpliCityBuildManager* USimpliCityFunctionLibrary::GetBuildManager(const UObject* WorldContextObject) {
  return Cast<ASimpliCityBuildManager>(UGameplayStatics::GetActorOfClass(WorldContextObject,ASimpliCityBuildManager::StaticClass()));
}

AMarkerManager* USimpliCityFunctionLibrary::GetMarkerManager(const UObject* WorldContextObject) {
  return Cast<AMarkerManager>(UGameplayStatics::GetActorOfClass(WorldContextObject,AMarkerManager::StaticClass()));
}

ASimpliCityRoadManager* USimpliCityFunctionLibrary::GetRoadManager(const UObject* WorldContextObject) {
  return Cast<ASimpliCityRoadManager>(UGameplayStatics::GetActorOfClass(WorldContextObject,ASimpliCityRoadManager::StaticClass()));
}

bool USimpliCityFunctionLibrary::AreLocationsEqual(FVector LocationA, FVector LocationB) {
  FVector ZAdjustedA = FVector(LocationA.X,LocationA.Y,0.0);
  FVector ZAdjustedB = FVector(LocationB.X,LocationB.Y,0.0);
  return ZAdjustedA.Equals(ZAdjustedB, 2);
}

float USimpliCityFunctionLibrary::GenerateCurveBetweenPoints(const TArray<FVector> Points, int32 NumPoints, TArray<FVector>& OutPoints) {
  NumPoints = FMath::Max(NumPoints, 2); // has to be above >=2
  return FVector::EvaluateBezier(Points.GetData(),NumPoints,OutPoints);
}
//static T EvaluateBezier(const TVector<T>* ControlPoints,int32 NumPoints,TArray<TVector<T>>& OutPoints);
//static TArray<FVector> EvaluateBezier(const FVector* ControlPoints,int32 NumPoints,TArray<FVector>& OutPoints);