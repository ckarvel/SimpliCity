// Property of Carina Karvel

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PathFinderInterface.h"
#include "GridManager.generated.h"

// minimalapi exports some key auto-generated functions (such as GetPrivateStaticClass) 
UCLASS(Blueprintable)
class GRIDSYSTEM_API AGridManager : public AActor, public IPathFinderInterface {
  GENERATED_BODY()

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
  // Sets default values for this actor's properties
  AGridManager();

  UFUNCTION(BlueprintPure)
    FORCEINLINE float GetGridWidth() const { return NumCols * TileSize; }

  UFUNCTION(BlueprintPure)
    FORCEINLINE float GetGridHeight() const { return NumRows * TileSize; }

  UFUNCTION(BlueprintPure)
    FORCEINLINE int GetNumCols() const { return NumCols; }

  UFUNCTION(BlueprintPure)
    FORCEINLINE int GetNumRows() const { return NumRows; }

  //////////////////////////////////////////////////////////////////////////////////////////////

  UFUNCTION(BlueprintPure,Category = "GridManager")
    void LocationToTile(FVector Location,int& OutRow,int& OutCol) const;

  UFUNCTION(BlueprintPure,Category = "GridManager")
    FVector LocationToCenter(FVector Location) const;

  UFUNCTION(BlueprintPure,Category = "GridManager")
    FVector TileToLocation(int Row,int Col,bool Center=true) const;

  UFUNCTION(BlueprintPure,Category = "GridManager")
    int32 LocationToIndex(FVector Location) const;

  UFUNCTION(BlueprintPure,Category = "GridManager")
    FVector IndexToLocation(int32 Index,bool Center=true) const;

  UFUNCTION(BlueprintPure,Category = "GridManager")
    void IndexToTile(int32 Index,int& OutRow,int& OutCol) const;

  UFUNCTION(BlueprintPure,Category = "GridManager")
    int32 TileToIndex(int Row,int Col) const;

  //////////////////////////////////////////////////////////////////////////////////////////////

  UFUNCTION(BlueprintPure,Category = "GridManager")
    bool IsIndexValid(int32 Index) const;

  UFUNCTION(BlueprintPure,Category = "GridManager")
    bool IsLocationValid(FVector Location) const;

  UFUNCTION(BlueprintPure,Category = "GridManager")
    bool IsTileValid(int Row,int Col) const;

  //////////////////////////////////////////////////////////////////////////////////////////////

  UFUNCTION(BlueprintCallable,Category = "GridManager")
    void GetNeighborIndexes(int32 index,TArray<int32>& Indexes) const;

  // gets indexes in order: left, right, top, bottom. can return -1
  // idx of -1 means it doesn't exist (on boundary)
  void GetNeighborIndexes_Unsafe(int32 Index,TArray<int32>& Neighbors) const;

  UFUNCTION(BlueprintCallable,Category = "GridManager")
  virtual TArray<FVector> GetNeighbors(FVector Location) const override;

  UFUNCTION(BlueprintCallable,Category = "GridManager")
  virtual float GetCost(FVector Location) const override { return 1.0; } // todo

  //////////////////////////////////////////////////////////////////////////////////////////////

protected:
  UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "GridManager")
  int NumRows;
  UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "GridManager")
  int NumCols;
  UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "GridManager|Geometry")
  float TileSize;
  UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "GridManager|Geometry")
  float LineThickness;
  UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "GridManager|Geometry")
  float LineOpacity;
  UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "GridManager|Geometry")
  float SelectionOpacity;
  UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "GridManager|Geometry")
  class UMaterialInstance* GridMaterial;
  UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = "GridManager|Geometry")
  class UProceduralMeshComponent* LineMeshComponent;
  UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = "GridManager|Geometry")
  class UProceduralMeshComponent* SelectionMeshComponent;
  UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "GridManager|Geometry")
  FLinearColor LineColor;
  UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "GridManager|Geometry")
  FLinearColor SelectionColor;
  UFUNCTION(BlueprintCallable,CallInEditor,Category = "GridManager|Geometry")
  void ShowGridLines();
  UFUNCTION(BlueprintCallable,CallInEditor,Category = "GridManager|Geometry")
  void HideGridLines();
  UFUNCTION(BlueprintCallable,Category = "GridManager|Geometry")
  void GenerateGridLayout();

private:
  UMaterialInstanceDynamic* CreateMaterialInstance(FLinearColor color,float opacity);
  void CreateLinesOnX(TArray<FVector>& vertices,TArray<int>& triangles);
  void CreateLinesOnY(TArray<FVector>& vertices,TArray<int>& triangles);
  void CreateSelectionLine(TArray<FVector>& vertices,TArray<int>& triangles);
  void CreateLine(FVector start,FVector end,float thickness,TArray<FVector>& vertices,TArray<int>& triangles);
  void CreateLineProceduralMesh(UProceduralMeshComponent* mesh,const TArray<FVector>& vertices,const TArray<int>& triangles);
  void CreateSelectionProceduralMesh(UProceduralMeshComponent* mesh,const TArray<FVector>& vertices,const TArray<int>& triangles);
};