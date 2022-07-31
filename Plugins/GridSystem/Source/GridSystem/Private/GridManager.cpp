// Property of Carina Karvel

#include "GridManager.h"
#include "GridSystem.h"
#include "GridCell.h"

#include "ProceduralMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/KismetMaterialLibrary.h"

//#include "MyAStarPathFinder.h"
//#include "PriorityQueueImpl.h"
//#include "UtilityLibrary.h"

// Sets default values
AGridManager::AGridManager()
  : NumRows(10)
  ,NumCols(10)
  ,TileSize(200.0)
  ,LineThickness(10.0)
  ,LineOpacity(1.0)
  ,SelectionOpacity(0.5) {
  // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;
  LineMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>("LineMesh");
  SetRootComponent(LineMeshComponent);
  SelectionMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>("SelectionMesh");
}

void AGridManager::BeginPlay() {
  Super::BeginPlay();
  GridCellActors.Init(nullptr,NumRows*NumCols);
}

//////////////////////////////////////////////////////////////////////////////////////////////

void AGridManager::LocationToTile(FVector Location,int& OutRow,int& OutCol) const {
  FVector Origin = GetActorLocation();
  float X = (Location.X - Origin.X) / GetGridHeight();
  OutRow = FMath::Clamp(NumRows * X,0,(NumRows-1));
  float Y =  (Location.Y - Origin.Y) / GetGridWidth();
  OutCol = FMath::Clamp(NumCols * Y,0,(NumCols-1));
}

FVector AGridManager::LocationToCenter(FVector Location) const {
  int OutRow,OutCol;
  LocationToTile(Location,OutRow,OutCol);
  return TileToLocation(OutRow,OutCol);
}

FVector AGridManager::TileToLocation(int Row,int Col,bool Center) const {
  FVector Origin = GetActorLocation();
  // if center is true, set to half tile size, else set to 0
  int CenterAdjust = Center == true ? (TileSize / 2) : 0;

  float X = (Row * TileSize) + Origin.X + CenterAdjust;
  float Y = (Col * TileSize) + Origin.Y + CenterAdjust;
  float Z = 0;
  return FVector(X,Y,Z);
}

int32 AGridManager::LocationToIndex(FVector Location) const {
  int OutRow,OutCol;
  LocationToTile(Location,OutRow,OutCol);
  return TileToIndex(OutRow,OutCol);
}

FVector AGridManager::IndexToLocation(int32 Index,bool Center) const {
  int OutRow,OutCol;
  IndexToTile(Index,OutRow,OutCol);
  return TileToLocation(OutRow,OutCol,Center);
}

void AGridManager::IndexToTile(int32 Index,int& OutRow,int& OutCol) const {
  OutRow = Index / NumCols;
  OutCol = Index % NumCols;
}

int32 AGridManager::TileToIndex(int Row,int Col) const {
  return (NumCols * Row + Col);
}

bool AGridManager::IsIndexValid(int Index) const {
  return (Index >= 0 && Index < NumRows* NumCols);
}

bool AGridManager::IsLocationValid(FVector Location) const {
  if (Location.X < 0 || Location.Y < 0)
    return false;
  int OutRow,OutCol;
  LocationToTile(Location,OutRow,OutCol);
  return IsTileValid(OutRow,OutCol);
}

bool AGridManager::IsTileValid(int Row,int Col) const {
  return (Row < NumRows && Col < NumCols) && (Row >= 0 && Col >= 0);
}
//////////////////////////////////////////////////////////////////////////////////////////////

void AGridManager::GetNeighborIndexes(int32 Index,TArray<int32>& Neighbors) const {
  Neighbors.Empty();
  if (IsIndexValid(Index) == false) {
    //UtilityLibrary::Warning("Location invalid" + FString(__FUNCTION__));
    return;
  }
  // just in case a boundary cell (only affects left and right)
  // divide by num of cols
  bool onLeftBoundary = Index % NumCols == 0;
  bool onRightBoundary = Index % NumCols == (NumCols - 1);

  int leftIdx = Index - 1;
  int rightIdx = Index + 1;
  int topIdx = Index + NumCols;
  int bottomIdx = Index - NumCols;
  if (IsIndexValid(leftIdx) && onLeftBoundary == false)
    Neighbors.Add(leftIdx);
  if (IsIndexValid(rightIdx) && onRightBoundary == false)
    Neighbors.Add(rightIdx);
  if (IsIndexValid(topIdx))
    Neighbors.Add(topIdx);
  if (IsIndexValid(bottomIdx))
    Neighbors.Add(bottomIdx);
  return;
}

void AGridManager::GetNeighborIndexes_Unsafe(int32 Index,TArray<int32>& Neighbors) const {
  if (IsIndexValid(Index) == false) {
    //UtilityLibrary::Warning("Location invalid" + FString(__FUNCTION__));
    return;
  }
  Neighbors.Empty();
  Neighbors.Append({ -1, -1, -1, -1 });
  // just in case a boundary cell (only affects left and right)
  // divide by num of cols
  bool onLeftBoundary = Index % NumCols == 0;
  bool onRightBoundary = Index % NumCols == (NumCols - 1);

  int leftIdx = Index - 1;
  int rightIdx = Index + 1;
  int topIdx = Index + NumCols;
  int bottomIdx = Index - NumCols;
  if (IsIndexValid(leftIdx) && onLeftBoundary == false)
    Neighbors[0] = leftIdx;
  if (IsIndexValid(rightIdx) && onRightBoundary == false)
    Neighbors[1] = rightIdx;
  if (IsIndexValid(topIdx))
    Neighbors[2] = topIdx;
  if (IsIndexValid(bottomIdx))
    Neighbors[3] = bottomIdx;
  return;
}

TArray<FVector> AGridManager::GetNeighbors(FVector Location) const {
  TArray<int32> NeighborIdxs;
  TArray<FVector> NeighborLocs;

  if (IsLocationValid(Location) == false) {
    //UtilityLibrary::Warning("Location invalid" + FString(__FUNCTION__));
    //TRACE_WARNING_PRINTF(LogGridSystem,"Location invalid");
    return NeighborLocs;
  }
  int32 idx = LocationToIndex(Location);
  GetNeighborIndexes(idx,NeighborIdxs);
  
  // convert indexes to locations
  for (auto nIdx : NeighborIdxs)     {
    NeighborLocs.Add(IndexToLocation(nIdx));
  }
  return NeighborLocs;
}

//////////////////////////////////////////////////////////////////////////////////////////////

void AGridManager::GenerateGridLayout() {
  if (GridMaterial == nullptr) {
    //TRACE_WARNING_PRINTF(LogGridSystem,"Grid Material invalid");
    return;
  }

  TArray<FVector> vertices;
  TArray<int> triangles;
  CreateLinesOnX(vertices,triangles);
  CreateLinesOnY(vertices,triangles);
  //UtilityLibrary::Debug("Vertices size: " + FString::FromInt(vertices.Num()));
  //UtilityLibrary::Debug("Triangles size: " + FString::FromInt(triangles.Num()));

  TArray<FVector> selection_vertices;
  TArray<int> selection_triangles;
  CreateSelectionLine(selection_vertices,selection_triangles);

  CreateLineProceduralMesh(LineMeshComponent,vertices,triangles);
  CreateSelectionProceduralMesh(SelectionMeshComponent,selection_vertices,selection_triangles);
}

void AGridManager::ShowGridLines() {
  LineMeshComponent->SetVisibility(true);
}

void AGridManager::HideGridLines() {
  LineMeshComponent->SetVisibility(false);
}

void AGridManager::CreateLineProceduralMesh(UProceduralMeshComponent* mesh,const TArray<FVector>& vertices,const TArray<int>& triangles) {
  UMaterialInstanceDynamic* LineMeshInstance = CreateMaterialInstance(LineColor,LineOpacity);
  TArray<FVector> normals;
  TArray<FVector2D> uvs;
  TArray<FLinearColor> vertexColors;
  TArray<FProcMeshTangent> tangents;

  int length = vertices.Num();
  normals.Init(FVector(0.0f,0.0f,1.0f),length);
  uvs.Init(FVector2D(0.0f,0.0f),length);
  vertexColors.Init(FLinearColor(0.0f,0.0f,0.0f,1.0f),length);
  tangents.Init(FProcMeshTangent(1.0f,0.0f,0.0f),length);

  bool collision = false;
  mesh->CreateMeshSection_LinearColor(0,vertices,triangles,normals,uvs,vertexColors,tangents,collision);
  mesh->SetMaterial(0,LineMeshInstance);
}

void AGridManager::CreateSelectionProceduralMesh(UProceduralMeshComponent* mesh,const TArray<FVector>& vertices,const TArray<int>& triangles) {
  UMaterialInstanceDynamic* SelectionMeshInstance = CreateMaterialInstance(SelectionColor,SelectionOpacity);
  TArray<FVector> normals;
  TArray<FVector2D> uvs;
  TArray<FLinearColor> vertexColors;
  TArray<FProcMeshTangent> tangents;

  int length = vertices.Num();
  normals.Init(FVector(0.0f,0.0f,1.0f),length);
  uvs.Init(FVector2D(0.0f,0.0f),length);
  vertexColors.Init(FLinearColor(0.0f,0.0f,0.0f,1.0f),length);
  tangents.Init(FProcMeshTangent(1.0f,0.0f,0.0f),length);

  bool collision = false;
  mesh->CreateMeshSection_LinearColor(0,vertices,triangles,normals,uvs,vertexColors,tangents,collision);
  mesh->SetMaterial(0,SelectionMeshInstance);
  mesh->SetVisibility(false); // unlike lines, we don't want selection tile to be visible until cursor hovering
}

UMaterialInstanceDynamic* AGridManager::CreateMaterialInstance(FLinearColor color,float opacity) {
  check(GridMaterial != nullptr)
  UMaterialInstanceDynamic* newMaterial = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this,GridMaterial);
  newMaterial->SetVectorParameterValue("Color",color);
  newMaterial->SetScalarParameterValue("Opacity",opacity);
  return newMaterial;
}

void AGridManager::CreateSelectionLine(TArray<FVector>& vertices,TArray<int>& triangles) {
  float start = TileSize / 2.0;
  float end = GetGridWidth();
  FVector start_vect = FVector(0,start,0);
  FVector end_vect = FVector(TileSize,start,0);
  CreateLine(start_vect,end_vect,TileSize,vertices,triangles);
}

void AGridManager::CreateLinesOnX(TArray<FVector>& vertices,TArray<int>& triangles) {
  float start = 0.0;
  float end = GetGridWidth();
  for (int idx = 0; idx <= NumRows; idx++) {
    start = idx * TileSize;
    FVector start_vect = FVector(start,0,0);
    FVector end_vect = FVector(start,end,0);
    CreateLine(start_vect,end_vect,LineThickness,vertices,triangles);
  }
}

void AGridManager::CreateLinesOnY(TArray<FVector>& vertices,TArray<int>& triangles) {
  float start = 0.0;
  float end = GetGridHeight();
  for (int idx = 0; idx <= NumCols; idx++) {
    start = idx * TileSize;
    FVector start_vect = FVector(0,start,0);
    FVector end_vect = FVector(end,start,0);
    CreateLine(start_vect,end_vect,LineThickness,vertices,triangles);
  }
}

void AGridManager::CreateLine(FVector start,FVector end,float thickness,TArray<FVector>& vertices,TArray<int>& triangles) {
  float halfThickness = thickness / 2.0;
  FVector lineDir = end - start;
  lineDir.Normalize();

  // 1. Get direction for line thickness
  FVector thicknessDir = FVector::CrossProduct(lineDir,FVector(0,0,1.0));

  // Get vertex depends on how thick the line is; not too confident on what this is...
  FVector lineThicknessVect = thicknessDir * halfThickness;

  // 2. Add triangles
  float vertexCount = vertices.Num();

  // 2, 1, 0 and 2, 3, 1 (vertex order)
  triangles.Add(vertexCount + 2);
  triangles.Add(vertexCount + 1);
  triangles.Add(vertexCount + 0);
  triangles.Add(vertexCount + 2);
  triangles.Add(vertexCount + 3);
  triangles.Add(vertexCount + 1);

  // 3. Add 4 vertices that make up square in grid
  vertices.Add(start + lineThicknessVect);
  vertices.Add(end + lineThicknessVect);
  vertices.Add(start - lineThicknessVect);
  vertices.Add(end - lineThicknessVect);
}