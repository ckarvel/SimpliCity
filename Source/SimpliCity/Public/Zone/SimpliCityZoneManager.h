// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SimpliCity.h"
#include "SimpliCityBaseManager.h"
#include "SimpliCityZoneBase.h"
#include "SimpliCityZoneType.h"
#include "SimpliCityZoneManager.generated.h"

class ASimpliCityZoneBase;
class ASimpliCityBuildingBase;

UCLASS(Blueprintable)
class SIMPLICITY_API ASimpliCityZoneManager : public ASimpliCityBaseManager {
  GENERATED_BODY()

public:
  ASimpliCityZoneManager();
  virtual void Enable(ESimpliCityResourceType _TypeId) override;
  virtual void Disable() override;

  UFUNCTION(BlueprintCallable, Category = "SimpliCityZoneManager")
  void DestroyObjects(const TArray<ASimpliCityObjectBase*>& ObjectList);

  UFUNCTION(BlueprintCallable, Category = "SimpliCityZoneManager")
  ESimpliCityZoneType GetZoneTypeAtLocation(FVector Location);
  UFUNCTION(BlueprintCallable, Category = "SimpliCityZoneManager")
  TArray<ASimpliCityBuildingBase*> GetAllBuildingsOfType(ESimpliCityZoneType Type);

  UFUNCTION(BlueprintCallable, Category = "SimpliCityZoneManager")
  UMaterialInstance* GetTypeMaterial(ESimpliCityZoneType ZoneType);

  void AddBuildingToList(ASimpliCityZoneBase* Zone, ASimpliCityBuildingBase* Building);
  void RemoveBuildingFromList(ASimpliCityBuildingBase* Building);

  UFUNCTION(BlueprintCallable, Category = "SimpliCityZoneManager")
  TArray<ASimpliCityZoneBase*> GetEmptyZones();
  void TrySpawningBuildings();

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
  void GetBuildType(ESimpliCityResourceType TypeId, ESimpliCityZoneType& OutType);

  FTimerHandle SpawnTimerHandle;

private:
  void InitializeCellZones();
  bool SaveLastCellState(ASimpliCityZoneBase* Zone);
  void ReloadCellState(ASimpliCityZoneBase* Zone);
  void ReloadAllCellStates();
  void ResetCellStates();

public:
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SimpliCityZoneManager")
  class UMaterialInstance* DefaultMaterial;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SimpliCityZoneManager")
  class UMaterialInstance* ResidentialMaterial;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SimpliCityZoneManager")
  class UMaterialInstance* CommercialMaterial;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SimpliCityZoneManager")
  class UMaterialInstance* IndustrialMaterial;

private:
  class ASimpliCityObjectSelector* ObjectSelector;

  TArray<ASimpliCityZoneBase*> ZoneGrid;
  
  TMap<ESimpliCityZoneType, TArray<ASimpliCityZoneBase*>> ZonesPerType;
  TMap<ASimpliCityBuildingBase*, ASimpliCityZoneBase*> BuildingToZoneMap;

  TMap<ESimpliCityResourceType, ESimpliCityZoneType> BuildIconToType;
  TMap<ASimpliCityZoneBase*, ESimpliCityResourceType> LastZoneStateMap;
  TSet<ASimpliCityZoneBase*> oldZones;

};
