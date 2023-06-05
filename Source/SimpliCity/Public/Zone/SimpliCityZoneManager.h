// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SimpliCity.h"
#include "SimpliCityBaseManager.h"
#include "SimpliCityZoneBase.h"
#include "SimpliCityZoneType.h"
#include "SimpliCityZoneManager.generated.h"

UCLASS(Blueprintable)
class SIMPLICITY_API ASimpliCityZoneManager : public ASimpliCityBaseManager {
  GENERATED_BODY()

public:
  ASimpliCityZoneManager();

protected:
  class ASimpliCityObjectSelector* ObjectSelector;
  virtual void BeginPlay() override;
  UFUNCTION(BlueprintCallable, Category = "SimpliCityZoneManager")
  virtual void StartBuilding() override;
  UFUNCTION(BlueprintCallable, Category = "SimpliCityZoneManager")
  virtual bool UpdateBuilding(FVector Location) override;
  UFUNCTION(BlueprintCallable, Category = "SimpliCityZoneManager")
  virtual void FinishBuilding() override;
  UFUNCTION(BlueprintCallable, Category = "SimpliCityZoneManager")
  virtual void CancelBuilding() override;

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SimpliCityZoneManager")
  void GetBuildType(UTexture2D* Icon, TEnumAsByte<ESimpliCityZoneType>& OutType);

  TEnumAsByte<ESimpliCityZoneType> BuildType;
  TSet<ASimpliCityZoneCell*> oldCells;

public:
  UFUNCTION(BlueprintCallable, Category = "SimpliCityZoneManager")
  virtual void Enable(UTexture2D* NewIcon) override;
  UFUNCTION(BlueprintCallable, Category = "SimpliCityZoneManager")
  virtual void Disable() override;
  UFUNCTION(BlueprintCallable, Category = "SimpliCityZoneManager")
  void InitializeCellZones();

  UFUNCTION(BlueprintCallable, Category = "SimpliCityZoneManager")
  bool SaveLastCellState(ASimpliCityZoneCell* Cell);

  UFUNCTION(BlueprintCallable, Category = "SimpliCityZoneManager")
  void ReloadCellState(ASimpliCityZoneCell* Cell);

  UFUNCTION(BlueprintCallable, Category = "SimpliCityZoneManager")
  void ReloadAllCellStates();

  UFUNCTION(BlueprintCallable, Category = "SimpliCityZoneManager")
  void ResetCellStates();

  UFUNCTION(BlueprintCallable, Category = "SimpliCityZoneManager")
  TEnumAsByte<ESimpliCityZoneType> GetZoneTypeAtLocation(FVector Location);

  UFUNCTION(BlueprintCallable, Category = "SimpliCityZoneManager")
  bool PlacePermanentZoneBase(ASimpliCityZoneBase* ZoneBase);

  UFUNCTION(BlueprintCallable, Category = "SimpliCityZoneManager")
  void DestroyObjects(const TArray<ASimpliCityObjectBase*>& ObjectList);

  UFUNCTION(BlueprintCallable, Category = "SimpliCityZoneManager")
  TArray<ASimpliCityZoneBase*> GetAllZoneBasesOfType(ESimpliCityZoneType Type);
  void AddZoneBaseToList(ESimpliCityZoneType Type, ASimpliCityZoneBase* ZoneBase);
  void RemoveZoneBaseFromList(ASimpliCityZoneBase* ZoneBase);

  UPROPERTY(EditAnywhere, NoClear, BlueprintReadOnly, Category = "SimpliCityZoneManager")
  TSubclassOf<ASimpliCityZoneCell> ZoneCellClass;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SimpliCityZoneManager")
  TArray<ASimpliCityZoneCell*> GridCells;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SimpliCityZoneManager")
  TMap<ASimpliCityZoneCell*, TEnumAsByte<ESimpliCityZoneType>> LastCellStateMap;

  TMap<ESimpliCityZoneType, TArray<ASimpliCityZoneBase*>> ZoneBasesPerType;
};
