// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Building/SimpliCityBuildingType.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SimpliCityBaseManager.h"
#include "SimpliCityBuildingManager.generated.h"

UCLASS(Blueprintable)
class SIMPLICITY_API ASimpliCityBuildingManager : public ASimpliCityBaseManager {
  GENERATED_BODY()

public:
  ASimpliCityBuildingManager();

protected:
  UFUNCTION(BlueprintCallable, Category = "SimpliCityBuildingManager")
  virtual bool UpdateBuilding(FVector Location) override;
  UFUNCTION(BlueprintCallable, Category = "SimpliCityBuildingManager")
  virtual void StartBuilding() override;
  UFUNCTION(BlueprintCallable, Category = "SimpliCityBuildingManager")
  virtual void FinishBuilding() override;
  UFUNCTION(BlueprintCallable, Category = "SimpliCityBuildingManager")
  virtual void CancelBuilding() override;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SimpliCityBuildingManager")
  ASimpliCityObjectBase* ActiveObject;

public:
  UFUNCTION(BlueprintCallable, Category = "SimpliCityBuildingManager")
  virtual void Enable(UTexture2D* NewIcon) override;
  UFUNCTION(BlueprintCallable, Category = "SimpliCityBuildingManager")
  virtual void Disable() override;

  UFUNCTION(BlueprintCallable, Category = "SimpliCityBuildingManager")
  bool PlacePermanentBuilding(ASimpliCityBuildingBase* Building);
  virtual ASimpliCityObjectBase* PlacePermanentObject(TSubclassOf<ASimpliCityObjectBase> ObjectClass,
                                                      const FVector Location, const FRotator Rotation) override;

  UFUNCTION(BlueprintCallable, Category = "SimpliCityBuildingManager")
  virtual void DestroyObjects(const TArray<ASimpliCityObjectBase*>& ObjectList) override;

  UFUNCTION(BlueprintCallable, Category = "SimpliCityBuildingManager")
  TArray<class ASimpliCityBuildingBase*> GetAllBuildingsOfType(ESimpliCityBuildingType Type);
  void AddBuildingToList(ESimpliCityBuildingType Type, ASimpliCityBuildingBase* Building);
  void RemoveBuildingFromList(ASimpliCityBuildingBase* Building);
  TMap<ESimpliCityBuildingType, TArray<ASimpliCityBuildingBase*>> BuildingListPerType;
};
