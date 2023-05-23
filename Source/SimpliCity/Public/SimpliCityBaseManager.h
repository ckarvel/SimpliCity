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

  // if a temporary object being placed currently exists in the world
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SimpliCityBaseManager")
  bool CurrentlyBuilding;

  FVector StartLocation;

  TArray<ASimpliCityObjectBase*> TemporaryObjectList;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpliCityBaseManager")
  TArray<ASimpliCityObjectBase*> PermanentObjectList;

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SimpliCityBaseManager")
  void Update();
  virtual void Update_Implementation();
  virtual void StartBuilding(FVector Location);
  virtual void FinishBuilding();
  virtual void CancelBuilding();

public:
  // Called every frame
  virtual void Tick(float DeltaTime) override;

  virtual USimpliObjectBase* CreateObject(ESimpliCityObjectType ObjectType, const FVector Location);
  UFUNCTION(BlueprintCallable, Category = "SimpliCityBaseManager")
  virtual ASimpliCityObjectBase* PlacePermanentObject(TSubclassOf<ASimpliCityObjectBase> ObjectClass,
                                                      const FVector Location, const FRotator Rotation);

  UFUNCTION(BlueprintCallable, Category = "SimpliCityBaseManager")
  virtual void DestroyObject(ASimpliCityObjectBase* Object);

  UFUNCTION(BlueprintCallable, Category = "SimpliCityBaseManager")
  virtual void DestroyObjects(const TArray<ASimpliCityObjectBase*>& ObjectList);
};
