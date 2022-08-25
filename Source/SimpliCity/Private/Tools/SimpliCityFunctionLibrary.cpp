// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/SimpliCityFunctionLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "GridManager.h"
#include "MarkerManager.h"
#include "MyAStarPathFinder.h"

#include "SimpliCityGameInstance.h"
#include "SimpliCityPlayerController.h"

#include "Builder/SimpliCityBuildManager.h"
#include "Road/SimpliCityRoadManager.h"

AGridManager* USimpliCityFunctionLibrary::GetGridManager(const UObject* WorldContextObject) {
  //USimpliCityGameInstance* gameInst = Cast<USimpliCityGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
  //check(gameInst != nullptr)
  //return gameInst->GetGridManager();
  return Cast<AGridManager>(UGameplayStatics::GetActorOfClass(WorldContextObject,AGridManager::StaticClass()));
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

bool USimpliCityFunctionLibrary::AreLocationsEqual(FVector LocationA, FVector LocationB, float Tolerance) {
  FVector ZAdjustedA = FVector(LocationA.X,LocationA.Y,0.0);
  FVector ZAdjustedB = FVector(LocationB.X,LocationB.Y,0.0);
  return ZAdjustedA.Equals(ZAdjustedB,Tolerance);
}

TArray<FVector> USimpliCityFunctionLibrary::GetPathBetween(UObject* Graph,FVector Start,FVector End) {
  check(Graph != nullptr);
  TArray<FVector> newPath = MyAStarPathFinder::AStarSearch(Graph,Start,End);
  return newPath;
}

FVector USimpliCityFunctionLibrary::VInterpTo(FVector Current,FVector Target,float Delta,float Speed) {
  return FMath::VInterpTo(Current,Target,Delta,Speed);
}

FRotator USimpliCityFunctionLibrary::RInterpTo(FRotator Current, FRotator Target,float Delta,float Speed) {
  return FMath::RInterpTo(Current,Target,Delta,Speed);
}

float USimpliCityFunctionLibrary::FInterpTo(float Current,float Target,float Delta,float Speed) {
  return FMath::FInterpTo(Current,Target,Delta,Speed);
}

bool USimpliCityFunctionLibrary::IsNearlyEqual(float A,float B) {
  return FMath::IsNearlyEqual(A, B, 0.1);
}