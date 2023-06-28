// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SimpliCity.h"
#include "SimpliCityObjectBase.h"
#include "SimpliCityObjectType.h"
#include "SimpliCityBaseManager.generated.h"

class USimpliObjectBase;
class AGridManager;
class ASimpliCityObjectManager;

UCLASS()
class SIMPLICITY_API ASimpliCityBaseManager : public AActor {
  GENERATED_BODY()

public:
  // Sets default values for this actor's properties
  ASimpliCityBaseManager();

  ASimpliCityObjectBase* GetTemporaryObjectAtLocation(FVector Location);

protected:
  virtual void Tick(float DeltaTime) override;
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;
  AGridManager* GridManager;
  ASimpliCityObjectManager* ObjectManager;
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SimpliCityBaseManager")
  bool BuildEnabled;
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SimpliCityBaseManager")
  bool CurrentlyBuilding;
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SimpliCityBaseManager")
  TArray<ASimpliCityObjectBase*> PermanentObjectList;

  UTexture2D* BuildIcon;
  FVector StartLocation;
  FVector LastLocation;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SimpliCityBaseManager")
  TMap<FVector, ASimpliCityObjectBase*> Temporary_ObjectToLocation;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpliCityBaseManager")
  TSubclassOf<ASimpliCityObjectBase> DefaultBlueprintClass;

  virtual void StartBuilding();
  virtual void Update(FVector Location){};
  virtual bool UpdateBuilding(FVector Location);
  virtual void FinishBuilding();
  virtual void CancelBuilding();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SimpliCityBaseManager")
  ASimpliCityObjectBase* SpawnObjectOfType(TSubclassOf<ASimpliCityObjectBase> Class, const FVector Location,
                                           const FRotator Rotation, UTexture2D* Icon);

public:

  UFUNCTION(BlueprintCallable, Category = "SimpliCityBaseManager")
  virtual void DestroyObject(ASimpliCityObjectBase* Object);
  virtual void DestroyObjects(const TArray<ASimpliCityObjectBase*>& ObjectList);

  UFUNCTION(BlueprintPure, Category = "SimpliCityBaseManager")
  bool IsEnabled() {
    return BuildEnabled;
  }

  virtual void Enable(UTexture2D* NewIcon) {
    if (NewIcon) {
      BuildEnabled = true;
      BuildIcon = NewIcon;
    }
  }

  virtual void Disable() {
    BuildEnabled = false;
    BuildIcon = nullptr;
  }

  UFUNCTION(BlueprintPure, Category = "SimpliCityBaseManager")
  bool IsActivelyBuilding() {
    return CurrentlyBuilding;
  }
};
