// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SimpliCityGameMode.generated.h"

UCLASS(minimalapi)
class ASimpliCityGameMode : public AGameModeBase {
  GENERATED_BODY()

public:
  ASimpliCityGameMode();
  virtual void StartPlay() override;

  // UPROPERTY(EditAnywhere,NoClear,BlueprintReadOnly,Category = "SimpliCityGameMode")
  // TSubclassOf<class AGridManager> GridManagerClass;

  /*UPROPERTY(EditAnywhere,NoClear,BlueprintReadOnly,Category = "SimpliCityGameMode")
  TSubclassOf<class USimpliCityMainUI> BuildUIClass;*/
};
