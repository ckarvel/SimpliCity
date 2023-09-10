// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SimpliCityObjectType.h"
#include "ObjectPlacer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlacementError);

class ASimpliCityGameState;
class AGridManager;
class AObjectManager;
class AObjectBase;
class ASimpliCityObjectSelector;

UCLASS(Blueprintable)
class SIMPLICITY_API AObjectPlacer : public AActor
{
  GENERATED_BODY()
  
public:	
  // Sets default values for this actor's properties
  AObjectPlacer();

  UFUNCTION(BlueprintPure, Category = "ObjectPlacer")
  ESimpliCityObjectType GetPlacingObjectType() {
    return ObjectType;
  }

  UFUNCTION(BlueprintPure, Category = "ObjectPlacer")
  ESimpliCityResourceType GetPlacingResourceType() {
    return ResourceType;
  }

  UFUNCTION(BlueprintPure, Category = "ObjectPlacer")
  bool IsEnabled() {
    return Enabled;
  }

  UFUNCTION(BlueprintCallable, Category = "ObjectPlacer")
  void Enable(bool shouldEnable, ESimpliCityObjectType objectType, ESimpliCityResourceType resourceType);

  UFUNCTION(BlueprintCallable, Category = "ObjectPlacer")
  void StartPlacement(ESimpliCityObjectType objectType, ESimpliCityResourceType resourceType);

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ObjectPlacer")
  AObjectBase* SpawnObject(ESimpliCityObjectType objectType, ESimpliCityResourceType resourceType, FVector Location);

  UFUNCTION(BlueprintImplementableEvent, Category = "ObjectPlacer")
  void UpdatePlacement(FVector Location);

  UFUNCTION(BlueprintCallable, Category = "ObjectPlacer")
  bool FinishPlacement(bool UserCancelled);

  UFUNCTION(BlueprintPure, Category = "ObjectPlacer")
  bool IsPlacementInProgress() {
    return IsPlacing;
  }

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ObjectPlacer")
  bool IsPlacementValid();

  UPROPERTY(BlueprintAssignable, Category = "ObjectPlacer")
  FOnPlacementError OnPlacementError;

  UFUNCTION(BlueprintCallable, Category = "ObjectPlacer")
  AObjectBase* GetTemporaryObject(FVector Location);

protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

  void Cleanup();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ObjectPlacer")
  void ConvertTemporariesToPermanent();

  UPROPERTY(BlueprintReadOnly, Category = "ObjectPlacer")
  ASimpliCityGameState* GameState;

  UPROPERTY(BlueprintReadOnly, Category = "ObjectPlacer")
  AObjectManager* ObjectManager;

  UPROPERTY(BlueprintReadOnly, Category = "ObjectPlacer")
  AGridManager* GridManager;

  UPROPERTY(BlueprintReadOnly, Category = "ObjectPlacer")
  ASimpliCityObjectSelector* ObjectSelector;

  UPROPERTY(BlueprintReadWrite, Category = "ObjectPlacer")
  TMap<FVector, AObjectBase*> TemporaryObjectLocationMap;

  bool Enabled;

  UPROPERTY(BlueprintReadWrite, Category = "ObjectPlacer")
  bool IsPlacing;

  ESimpliCityObjectType ObjectType;
  ESimpliCityResourceType ResourceType;

  UPROPERTY(BlueprintReadWrite, Category = "ObjectPlacer")
  FVector StartLocation;
  UPROPERTY(BlueprintReadOnly, Category = "ObjectPlacer")
  FVector LastLocation;

public:
  // Called every frame
  virtual void Tick(float DeltaTime) override;
};
