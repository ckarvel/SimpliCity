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

protected:
  virtual void Tick(float DeltaTime) override;
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;
  AGridManager* GridManager;
  ASimpliCityObjectManager* ObjectManager;
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASimpliCityBaseManager")
  bool BuildEnabled;
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASimpliCityBaseManager")
  bool CurrentlyBuilding;
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASimpliCityBaseManager")
  TArray<ASimpliCityObjectBase*> PermanentObjectList;

  UTexture2D* BuildIcon;
  FVector StartLocation;
  FVector LastLocation;
  TMap<FVector, ASimpliCityObjectBase*> Temporary_ObjectToLocation;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASimpliCityBaseManager")
  TSubclassOf<ASimpliCityObjectBase> DefaultBlueprintClass;

  virtual void StartBuilding();
  virtual void Update(FVector Location){};
  virtual bool UpdateBuilding(FVector Location);
  virtual void FinishBuilding();
  virtual void CancelBuilding();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ASimpliCityBaseManager")
  ASimpliCityObjectBase* SpawnObjectOfType(TSubclassOf<ASimpliCityObjectBase> Class, const FVector Location,
                                           const FRotator Rotation, UTexture2D* Icon);

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
