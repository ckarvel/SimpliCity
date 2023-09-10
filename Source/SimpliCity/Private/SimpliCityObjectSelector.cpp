// Fill out your copyright notice in the Description page of Project Settings.

#include "SimpliCityObjectSelector.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "ProceduralMeshComponent.h"
#include "SimpliCityFunctionLibrary.h"
#include "GridManager.h"
#include "Zone/SimpliCityZoneBase.h"
#include "Algo/Reverse.h"

using SCFL = USimpliCityFunctionLibrary;

//////////////////////////////////////////////////////////////////////////
ASimpliCityObjectSelector::ASimpliCityObjectSelector()
    : VisualizeSelection(true) {
  PrimaryActorTick.bCanEverTick = false;
  SelectionMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>("SelectionMesh");
  SetRootComponent(SelectionMeshComponent);
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityObjectSelector::BeginPlay() {
  Super::BeginPlay();
  GridManager = SCFL::GetGridManager(this);
}

//////////////////////////////////////////////////////////////////////////
template <class SelectionClass>
TArray<SelectionClass*> ASimpliCityObjectSelector::UpdateSelection(FVector Start, FVector End) {
  TArray<SelectionClass*> SelectedObjects;
  FVector Extents;
  TArray<FVector> Vertices;
  TArray<int> Triangles;
  SCFL::CalculateSelectionRectangle(Start, End, Extents, Vertices, Triangles);

  // reset selection list
  SelectedObjects.Empty();

  // visualize selection area
  if (VisualizeSelection) {
    DrawSelection(Vertices, Triangles);
  }

  // store objects in selection area
  FVector BoxPos = SCFL::GetMidpointBetween(Start, End);
  FVector BoxExtent = FVector(Extents.X, Extents.Y, 100); // set height large enough to overlap world actors
  TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = {
      UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic)};
  TArray<AActor*> ActorsToIgnore = {this};
  TArray<AActor*> OutActors;
  UKismetSystemLibrary::BoxOverlapActors(this, BoxPos, BoxExtent, ObjectTypes, SelectionClass::StaticClass(),
                                         ActorsToIgnore,
                                         OutActors);
  for (AActor* Actor : OutActors) {
    SelectedObjects.Add(Cast<SelectionClass>(Actor));
  }
  return SelectedObjects;
}

//////////////////////////////////////////////////////////////////////////
TArray<FVector> ASimpliCityObjectSelector::GetBoundingBoxSelection(FVector Start, FVector End) {
  TArray<FVector> SelectedPoints;
  int StartRow, StartCol = 0;
  int EndRow, EndCol = 0;
  GridManager->LocationToTile(Start, StartRow, StartCol);
  GridManager->LocationToTile(End, EndRow, EndCol);

  // from start row, step row until you hit end's row
  // from start col, step until you hit end's col
  int rowIncrement = StartRow < EndRow ? 1 : -1;
  int colIncrement = StartCol < EndCol ? 1 : -1;
  int nextRow = StartRow;
  // iterate over rows & columns adding each point found
  while (nextRow != EndRow + rowIncrement) {
    int nextCol = StartCol; // reset column for next row
    while (nextCol != EndCol + colIncrement) {
      FVector Point = GridManager->TileToLocation(nextRow, nextCol);
      if (GridManager->IsLocationValid(Point)) {
        SelectedPoints.Add(Point);
      }
      nextCol += colIncrement;
    }
    nextRow += rowIncrement;
  }
  return SelectedPoints;
}

//////////////////////////////////////////////////////////////////////////
TArray<FVector> ASimpliCityObjectSelector::GetPathSelection(FVector Start, FVector End) {
  FVector StartCenter = GridManager->LocationToCenter(Start);
  FVector EndCenter = GridManager->LocationToCenter(End);
  return SCFL::GetPathBetween(GridManager, StartCenter, EndCenter);
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityObjectSelector::FinishSelection() {
  SelectionMeshComponent->ClearMeshSection(0);
  VisualizeSelection = true; // reset
}

//////////////////////////////////////////////////////////////////////////
UMaterialInstanceDynamic* ASimpliCityObjectSelector::CreateMaterialInstance() {
  if (SelectionMaterial == nullptr) {
    return nullptr;
  }
  UMaterialInstanceDynamic* NewMaterial = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, SelectionMaterial);
  NewMaterial->SetVectorParameterValue("Color", SelectionColor);
  NewMaterial->SetScalarParameterValue("Opacity", SelectionOpacity);
  return NewMaterial;
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityObjectSelector::DrawSelection(TArray<FVector> Vertices, TArray<int> Triangles) {
  UMaterialInstanceDynamic* MeshInstance = CreateMaterialInstance();
  TArray<FVector> normals;
  TArray<FVector2D> uvs;
  TArray<FLinearColor> vertexColors;
  TArray<FProcMeshTangent> tangents;

  int length = Vertices.Num();
  normals.Init(FVector(0.0f, 0.0f, 1.0f), length);
  uvs.Init(FVector2D(0.0f, 0.0f), length);
  vertexColors.Init(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f), length);
  tangents.Init(FProcMeshTangent(1.0f, 0.0f, 0.0f), length);

  bool collision = false;
  SelectionMeshComponent->CreateMeshSection_LinearColor(0, Vertices, Triangles, normals, uvs, vertexColors, tangents,
                                                        collision);
  SelectionMeshComponent->SetMaterial(0, MeshInstance);
  SelectionMeshComponent->SetVisibility(true);
}

template TArray<ASimpliCityZoneBase*> ASimpliCityObjectSelector::UpdateSelection(FVector Start, FVector End);