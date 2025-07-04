// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SimpliCity.h"
#include "SimpliCityObjectType.h"
#include "SimpliCityObjectManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpawnedObject, FVector, Location);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDestroyedObject, FVector, Location);

class ASimpliCityObjectBase;

UCLASS(Blueprintable)
class SIMPLICITY_API ASimpliCityObjectManager : public AActor {
  GENERATED_BODY()

public:
  ASimpliCityObjectManager();

  UFUNCTION(BlueprintCallable, Category = "SimpliCityObjectManager")
  void InitializeArray(int NumElements);

  UFUNCTION(BlueprintCallable, Category = "SimpliCityObjectManager")
  bool DoesObjectExistHere(FVector Location);

  UFUNCTION(BlueprintCallable, Category = "SimpliCityObjectManager")
  void AddObjectToGrid(ASimpliCityObjectBase *AddedObject);

  UFUNCTION(BlueprintCallable, Category = "SimpliCityObjectManager")
  void ReplaceObjectInGrid(ASimpliCityObjectBase *OldObject, ASimpliCityObjectBase *NewObject);

  UFUNCTION(BlueprintCallable, Category = "SimpliCityObjectManager")
  void RemoveObjectFromGrid(ASimpliCityObjectBase *RemovedObject);

  UFUNCTION(BlueprintCallable, Category = "SimpliCityObjectManager")
  void RemoveObjectAtLocation(FVector Location);

  UFUNCTION(BlueprintCallable, Category = "SimpliCityObjectManager")
  ASimpliCityObjectBase *GetObjectAtLocation(FVector Location);

  UFUNCTION(BlueprintCallable, Category = "SimpliCityObjectManager")
  TArray<ASimpliCityObjectBase *> GetNeighborsOfType(FVector Location, ESimpliCityObjectType ObjectType);

  UFUNCTION(BlueprintCallable, Category = "SimpliCityObjectManager")
  TArray<ASimpliCityObjectBase *> GetAllNeighbors(FVector Location);

  UFUNCTION(BlueprintCallable, Category = "SimpliCityObjectManager")
  bool IsNearRoad(FVector Location);

  UFUNCTION(BlueprintCallable, Category = "SimpliCityObjectManager")
  bool IsInitialized() { return bInitialized; }

  UPROPERTY(BlueprintAssignable, Category = "SimpliCityObjectManager")
  FOnSpawnedObject OnSpawnedObject;

  UPROPERTY(BlueprintAssignable, Category = "SimpliCityObjectManager")
  FOnDestroyedObject OnDestroyedObject;

protected:
  virtual void BeginPlay() override;

public:
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SimpliCityObjectManager")
  TArray<ASimpliCityObjectBase *> ObjectGrid;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SimpliCityObjectManager")
  bool bInitialized;

  bool IsReplacing;
};
