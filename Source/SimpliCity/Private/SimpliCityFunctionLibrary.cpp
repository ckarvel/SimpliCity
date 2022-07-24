// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpliCityFunctionLibrary.h"
#include "SimpliCityGameInstance.h"
#include "SimpliCityPlayerController.h"
#include "SimpliCityBuildManager.h"
#include "GridManager.h"

#include "Kismet/GameplayStatics.h"

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

bool USimpliCityFunctionLibrary::AreLocationsEqual(FVector LocationA, FVector LocationB) {
  FVector ZAdjustedA = FVector(LocationA.X,LocationA.Y,0.0);
  FVector ZAdjustedB = FVector(LocationB.X,LocationB.Y,0.0);
  return ZAdjustedA.Equals(ZAdjustedB, 0.01);
}