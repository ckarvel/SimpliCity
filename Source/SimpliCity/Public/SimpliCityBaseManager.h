// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SimpliCity.h"
#include "SimpliCityObjectBase.h"
#include "SimpliCityObjectType.h"
#include "SimpliCityBaseManager.generated.h"

class USimpliObjectBase;

UCLASS()
class SIMPLICITY_API ASimpliCityBaseManager : public AActor {
  GENERATED_BODY()

public:
  // Sets default values for this actor's properties
  ASimpliCityBaseManager();

protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

  bool BuildEnabled; // set from user interface
  bool CurrentlyBuilding;

  FVector StartLocation;
  FVector LastLocation;
  TMap<FVector, ASimpliCityObjectBase*> Temporary_ObjectToLocation;
  TArray<ASimpliCityObjectBase*> PermanentObjectList;

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SimpliCityBaseManager")
  void Update();
  virtual void Update_Implementation();
  virtual void StartBuilding(FVector Location);
  virtual void FinishBuilding();
  virtual void CancelBuilding();

public:
  virtual USimpliObjectBase* CreateObject(TEnumAsByte<ESimpliCityObjectType> ObjectType, const FVector Location);

  virtual ASimpliCityObjectBase* PlaceObject(TSubclassOf<ASimpliCityObjectBase> ObjectClass, const FVector Location,
                                             const FRotator Rotation);

  virtual ASimpliCityObjectBase* PlaceTemporaryObject(TSubclassOf<ASimpliCityObjectBase> ObjectClass,
                                                      const FVector Location, const FRotator Rotation);

  UFUNCTION(BlueprintCallable, Category = "SimpliCityBaseManager")
  virtual ASimpliCityObjectBase* PlacePermanentObject(TSubclassOf<ASimpliCityObjectBase> ObjectClass,
                                                      const FVector Location, const FRotator Rotation);

  UFUNCTION(BlueprintCallable, Category = "SimpliCityBaseManager")
  virtual void DestroyObject(ASimpliCityObjectBase* Object);
  virtual void DestroyObjects(const TArray<ASimpliCityObjectBase*>& ObjectList);

  UFUNCTION(BlueprintPure, Category = "SimpliCityBaseManager")
  bool IsEnabled() {
    return BuildEnabled;
  }

  UFUNCTION(BlueprintCallable, Category = "SimpliCityBaseManager")
  void Enable() {
    BuildEnabled = true;
  }

  UFUNCTION(BlueprintCallable, Category = "SimpliCityBaseManager")
  void Disable() {
    BuildEnabled = false;
  }

  UFUNCTION(BlueprintPure, Category = "SimpliCityBaseManager")
  bool IsActivelyBuilding() {
    return CurrentlyBuilding;
  }
};
