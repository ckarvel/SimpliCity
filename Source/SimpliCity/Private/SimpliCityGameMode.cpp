// Copyright Epic Games, Inc. All Rights Reserved.

#include "SimpliCityGameMode.h"
#include "SimpliCityGameInstance.h"
#include "GridManager.h"
#include "Kismet/GameplayStatics.h"


ASimpliCityGameMode::ASimpliCityGameMode()
{
  GridManagerClass = AGridManager::StaticClass();
}

void ASimpliCityGameMode::StartPlay() {
  USimpliCityGameInstance* gameInst = Cast<USimpliCityGameInstance>(UGameplayStatics::GetGameInstance(this));
  check(gameInst != nullptr)
  gameInst->GridManager = GetWorld()->SpawnActor<AGridManager>(GridManagerClass);
  Super::StartPlay();
}