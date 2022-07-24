// Property of Carina Karvel

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PathFinderInterface.h"
#include "GridManager.generated.h"

// minimalapi exports some key auto-generated functions (such as GetPrivateStaticClass) 
UCLASS(Blueprintable,minimalapi)
class AGridManager : public AActor, public IPathFinderInterface {
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

  UFUNCTION(BlueprintPure,Category = "Grid")
    void LocationToTile(FVector Location,int& OutRow,int& OutCol) const;

  UFUNCTION(BlueprintPure,Category = "Grid")
    FVector LocationToCenter(FVector Location) const;

  UFUNCTION(BlueprintPure,Category = "Grid")
    FVector TileToLocation(int Row,int Col,bool Center=true) const;

  UFUNCTION(BlueprintPure,Category = "Grid")
    int32 LocationToIndex(FVector Location) const;

  UFUNCTION(BlueprintPure,Category = "Grid")
    FVector IndexToLocation(int32 Index,bool Center=true) const;

  UFUNCTION(BlueprintPure,Category = "Grid")
    void IndexToTile(int32 Index,int& OutRow,int& OutCol) const;

  UFUNCTION(BlueprintPure,Category = "Grid")
    int32 TileToIndex(int Row,int Col) const;

  //////////////////////////////////////////////////////////////////////////////////////////////

  UFUNCTION(BlueprintPure,Category = "Grid")
    bool IsIndexValid(int32 Index) const;

  UFUNCTION(BlueprintPure,Category = "Grid")
    bool IsLocationValid(FVector Location) const;

  UFUNCTION(BlueprintPure,Category = "Grid")
    bool IsTileValid(int Row,int Col) const;

  //////////////////////////////////////////////////////////////////////////////////////////////

  UFUNCTION(BlueprintCallable,Category = "Grid")
    void GetNeighborIndexes(int32 index,TArray<int32>& Indexes) const;

  UFUNCTION(BlueprintCallable,Category = "Grid")
  virtual TArray<FVector> GetNeighbors(FVector Location) const override;

  UFUNCTION(BlueprintCallable,Category = "Grid")
  virtual float GetCost(FVector Location) const override { return 1.0; } // todo

  //////////////////////////////////////////////////////////////////////////////////////////////

protected:
  UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = Grid)
    TArray<class AGridCell*> GridCellActors;
  UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Grid)
    int NumRows;
  UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Grid)
    int NumCols;
  UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = GridGeom)
    float TileSize;
  UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = GridGeom)
    float LineThickness;
  UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = GridGeom)
    float LineOpacity;
  UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = GridGeom)
    float SelectionOpacity;
  UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = GridGeom)
    class UMaterialInstance* GridMaterial;
  UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = GridGeom,meta = (AllowPrivateAccess = "true"))
    class UProceduralMeshComponent* LineMeshComponent;
  UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = GridGeom,meta = (AllowPrivateAccess = "true"))
    class UProceduralMeshComponent* SelectionMeshComponent;
  UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = GridGeom)
    FLinearColor LineColor;
  UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = GridGeom)
    FLinearColor SelectionColor;
  UFUNCTION(BlueprintCallable,CallInEditor,Category = GridGeom)
    void ShowGridLines();
  UFUNCTION(BlueprintCallable,CallInEditor,Category = GridGeom)
    void HideGridLines();
  UFUNCTION(BlueprintCallable,Category = GridGeom)
    void GenerateGridLayout();

private:
  //void SetSelectedTile(FVector location); // called by gridmanager
  UMaterialInstanceDynamic* CreateMaterialInstance(FLinearColor color,float opacity);
  void CreateLinesOnX(TArray<FVector>& vertices,TArray<int>& triangles);
  void CreateLinesOnY(TArray<FVector>& vertices,TArray<int>& triangles);
  void CreateSelectionLine(TArray<FVector>& vertices,TArray<int>& triangles);
  void CreateLine(FVector start,FVector end,float thickness,TArray<FVector>& vertices,TArray<int>& triangles);
  void CreateLineProceduralMesh(UProceduralMeshComponent* mesh,const TArray<FVector>& vertices,const TArray<int>& triangles);
  void CreateSelectionProceduralMesh(UProceduralMeshComponent* mesh,const TArray<FVector>& vertices,const TArray<int>& triangles);
};