// Copyright Epic Games, Inc. All Rights Reserved.

#include "SimpliCityGameMode.h"
#include "SimpliCityGameInstance.h"
// #include "SimpliCityPlayerController.h"
// #include "SimpliCityMainUI.h"
// #include "GridManager.h"
#include "Kismet/GameplayStatics.h"

ASimpliCityGameMode::ASimpliCityGameMode() {
  // GridManagerClass = AGridManager::StaticClass();
  // BuildUIClass = USimpliCityMainUI::StaticClass();
}

void ASimpliCityGameMode::StartPlay() {
  // USimpliCityGameInstance* gameInst = Cast<USimpliCityGameInstance>(UGameplayStatics::GetGameInstance(this));
  // gameInst->GridManager = GetWorld()->SpawnActor<AGridManager>(GridManagerClass);

  // ASimpliCityPlayerController* controller =
  // Cast<ASimpliCityPlayerController>(UGameplayStatics::GetPlayerController(this,0)); auto NewWidget =
  // CreateWidget<USimpliCityMainUI>(controller,BuildUIClass); NewWidget->AddToViewport(); controller->TheMainUI =
  // NewWidget;
  Super::StartPlay();
}
