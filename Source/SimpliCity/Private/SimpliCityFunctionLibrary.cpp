// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpliCityFunctionLibrary.h"
#include "SimpliCityGameInstance.h"
#include "SimpliCityPlayerController.h"
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