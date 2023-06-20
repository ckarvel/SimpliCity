// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SimpliCityObjectType.h"
#include "SimpliObjectBase.h"
#include "SimpliCityObjectBase.generated.h"

UCLASS(Blueprintable)
class SIMPLICITY_API ASimpliCityObjectBase : public AActor {
  GENERATED_BODY()

public:
  ASimpliCityObjectBase();
  UFUNCTION(BlueprintPure, Category = "SimpliCityObjectBase")
  FORCEINLINE TEnumAsByte<ESimpliCityObjectType> Type() const {
    return ObjectType;
  }

protected:
  virtual void BeginPlay() override;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpliCityObjectBase")
  class UMaterialInstance* ErrorMaterial; // incorrect placement, usually red

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpliCityObjectBase")
  class UMaterialInstance* GoodMaterial; // correct placement, usually green

public:
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SimpliCityObjectBase")
  class UStaticMeshComponent* StaticMeshComponent;

  UFUNCTION(BlueprintCallable, Category = "SimpliCityObjectBase")
  FORCEINLINE UStaticMeshComponent* GetStaticMeshComponent() const {
    return StaticMeshComponent;
  }

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SimpliCityObjectBase")
  UTexture2D* BuildIcon;

  virtual void SetNewMaterial(UMaterialInstance* Material);

  virtual void SetNewLocation(FVector Location);

  // this object is placed permanently on the grid, start any necessary component behaviors
  virtual void OnObjectPlaced();

protected:
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SimpliCityObjectBase")
  TEnumAsByte<ESimpliCityObjectType> ObjectType;
};
