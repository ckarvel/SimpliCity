// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SimpliCity.h"
#include "SimpliCityBaseManager.h"
#include "SimpliCityZoneBase.h"
#include "SimpliCityZoneType.h"
#include "SimpliCityZoneManager.generated.h"

class ASimpliCityZoneCell;

UCLASS(Blueprintable)
class SIMPLICITY_API ASimpliCityZoneManager : public ASimpliCityBaseManager {
  GENERATED_BODY()

public:
  ASimpliCityZoneManager();

  UFUNCTION(BlueprintCallable, Category = "SimpliCityZoneManager")
  virtual void Enable(UTexture2D* NewIcon) override;
  UFUNCTION(BlueprintCallable, Category = "SimpliCityZoneManager")
  virtual void Disable() override;

  UFUNCTION(BlueprintCallable, Category = "SimpliCityZoneManager")
  bool PlacePermanentZoneBase(ASimpliCityZoneBase* ZoneBase);
  virtual ASimpliCityObjectBase* PlacePermanentObject(TSubclassOf<ASimpliCityObjectBase> ObjectClass,
                                                      const FVector Location, const FRotator Rotation) override;

  UFUNCTION(BlueprintCallable, Category = "SimpliCityZoneManager")
  void DestroyObjects(const TArray<ASimpliCityObjectBase*>& ObjectList);

  UFUNCTION(BlueprintCallable, Category = "SimpliCityZoneManager")
  TEnumAsByte<ESimpliCityZoneType> GetZoneTypeAtLocation(FVector Location);
  UFUNCTION(BlueprintCallable, Category = "SimpliCityZoneManager")
  TArray<ASimpliCityZoneBase*> GetAllZoneBasesOfType(ESimpliCityZoneType Type);

  void AddZoneBaseToList(ESimpliCityZoneType Type, ASimpliCityZoneBase* ZoneBase);
  void RemoveZoneBaseFromList(ASimpliCityZoneBase* ZoneBase);

protected:
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

private:
  void InitializeCellZones();
  bool SaveLastCellState(ASimpliCityZoneCell* Cell);
  void ReloadCellState(ASimpliCityZoneCell* Cell);
  void ReloadAllCellStates();
  void ResetCellStates();


public:
protected:
  UPROPERTY(EditAnywhere, NoClear, BlueprintReadOnly, Category = "SimpliCityZoneManager")
  TSubclassOf<ASimpliCityZoneCell> ZoneCellClass;
  TMap<ESimpliCityZoneType, TArray<ASimpliCityZoneBase*>> ZoneBasesPerType;

private:
  TArray<ASimpliCityZoneCell*> GridCells;
  TMap<ASimpliCityZoneCell*, TEnumAsByte<ESimpliCityZoneType>> LastCellStateMap;
  class ASimpliCityObjectSelector* ObjectSelector;
  TEnumAsByte<ESimpliCityZoneType> BuildType;
  TSet<ASimpliCityZoneCell*> oldCells;
};
