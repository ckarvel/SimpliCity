// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SimpliCity.h"
#include "SimpliCityZoneBase.h"
#include "SimpliCityZoneType.h"
#include "SimpliCityZoneManager.generated.h"

UCLASS()
class SIMPLICITY_API ASimpliCityZoneManager : public AActor {
  GENERATED_BODY()

public:
  ASimpliCityZoneManager();

protected:
  virtual void BeginPlay() override;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SimpliCityZoneManager")
  bool IsCurrentlyBuilding;

public:
  UFUNCTION(BlueprintCallable, Category = "SimpliCityZoneManager")
  void InitializeCellZones();

  UFUNCTION(BlueprintCallable, Category = "SimpliCityZoneManager")
  bool SaveLastCellState(ASimpliCityZoneCell *Cell);

  UFUNCTION(BlueprintCallable, Category = "SimpliCityZoneManager")
  void ReloadCellState(ASimpliCityZoneCell *Cell);

  UFUNCTION(BlueprintCallable, Category = "SimpliCityZoneManager")
  void ReloadAllCellStates();

  UFUNCTION(BlueprintCallable, Category = "SimpliCityZoneManager")
  void ResetCellStates();

  UFUNCTION(BlueprintCallable, Category = "SimpliCityZoneManager")
  TEnumAsByte<ESimpliCityZoneType> GetZoneTypeAtLocation(FVector Location);

  UFUNCTION(BlueprintCallable, Category = "SimpliCityZoneManager")
  bool PlacePermanentZoneBase(ASimpliCityZoneBase *ZoneBase);

  UFUNCTION(BlueprintCallable, Category = "SimpliCityZoneManager")
  void DestroyObjects(const TArray<ASimpliCityObjectBase *> &ObjectList);

  UFUNCTION(BlueprintCallable, Category = "SimpliCityZoneManager")
  TArray<ASimpliCityZoneBase *> GetAllZoneBasesOfType(ESimpliCityZoneType Type);
  void AddZoneBaseToList(ESimpliCityZoneType Type, ASimpliCityZoneBase *ZoneBase);
  void RemoveZoneBaseFromList(ASimpliCityZoneBase *ZoneBase);

  UPROPERTY(EditAnywhere, NoClear, BlueprintReadOnly, Category = "SimpliCityZoneManager")
  TSubclassOf<ASimpliCityZoneCell> ZoneCellClass;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SimpliCityZoneManager")
  TArray<ASimpliCityZoneCell *> GridCells;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SimpliCityZoneManager")
  TMap<ASimpliCityZoneCell *, TEnumAsByte<ESimpliCityZoneType>> LastCellStateMap;

  TMap<ESimpliCityZoneType, TArray<ASimpliCityZoneBase *>> ZoneBasesPerType;
};
