// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SimpliCityObjectBase.h"
#include "SimpliCityObjectSelector.generated.h"

UCLASS(Blueprintable)
class SIMPLICITY_API ASimpliCityObjectSelector : public AActor {
  GENERATED_BODY()

public:
  ASimpliCityObjectSelector();

public:
  template <class SelectionClass>
  TArray<SelectionClass*> UpdateSelection(FVector Start, FVector End);

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SimpliCityObjectSelector")
  TArray<FVector> GetSelectedLocations(ESimpliCityObjectType Type, FVector Start, FVector End);

  UFUNCTION(BlueprintCallable, Category = "SimpliCityObjectSelector")
  TArray<FVector> GetBoundingBoxSelection(FVector Start, FVector End);
  UFUNCTION(BlueprintCallable, Category = "SimpliCityObjectSelector")
  TArray<FVector> GetPathSelection(FVector Start, FVector End);

  void FinishSelection();

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpliCityObjectSelector")
  bool VisualizeSelection;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpliCityObjectSelector")
  FLinearColor SelectionColor;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpliCityObjectSelector")
  float SelectionOpacity;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpliCityObjectSelector")
  class UMaterialInstance* SelectionMaterial;

protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

private:
  void DrawSelection(TArray<FVector> Vertices, TArray<int> Triangles);

  UMaterialInstanceDynamic* CreateMaterialInstance();

  class UProceduralMeshComponent* SelectionMeshComponent;

  class AGridManager* GridManager;
};
