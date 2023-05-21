// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SimpliCityGameInstance.generated.h"

class AGridManager;
/**
 *
 */
UCLASS()
class SIMPLICITY_API USimpliCityGameInstance : public UGameInstance {
  GENERATED_BODY()

public:
  //	AGridManager* GetGridManager() const { return GridManager; }
  // protected:
  //	AGridManager* GridManager;
  //
  //	friend class ASimpliCityGameMode;
};
