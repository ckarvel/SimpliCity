// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SimpliCityObjectType.h"
#include "ObjectManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewObject, FVector, Location);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRemovedObject, FVector, Location);

class AGridManager;
class AObjectBase;

UCLASS(Blueprintable)
class SIMPLICITY_API AObjectManager : public AActor
{
  GENERATED_BODY()
  
public:
  // Sets default values for this actor's properties
  AObjectManager();

  UPROPERTY(BlueprintAssignable, Category = "ObjectManager")
  FOnNewObject OnNewObject;

  UPROPERTY(BlueprintAssignable, Category = "ObjectManager")
  FOnRemovedObject OnRemovedObject;

protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

  AGridManager* GridManager;

public:
  UFUNCTION(BlueprintCallable, Category = "ObjectManager")
  void AddObject(AObjectBase* Object);

  UFUNCTION(BlueprintCallable, Category = "ObjectManager")
  void RemoveObject(AObjectBase* Object);

  UFUNCTION(BlueprintCallable, Category = "ObjectManager")
  TArray<AObjectBase*> GetObjects(FVector Location);

  UFUNCTION(BlueprintCallable, Category = "ObjectManager")
  TArray<AObjectBase*> GetObjectsOfType(ESimpliCityObjectType Type);

  UFUNCTION(BlueprintCallable, Category = "ObjectManager")
  TArray<AObjectBase*> GetNeighborsOfType(FVector Location, ESimpliCityObjectType ObjectType);
  
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SimpliCityObjectManager")
  TArray<AObjectBase*> ObjectGrid;
  TMap<FVector, TArray<AObjectBase*>> LocationObjectMap;
  TMap<ESimpliCityObjectType, TArray<AObjectBase*>> ObjectsPerTypeMap;
};
