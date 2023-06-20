// Fill out your copyright notice in the Description page of Project Settings.

#include "Zone/SimpliCityZoneManager.h"

#include "GridManager.h"
#include "SimpliCityFunctionLibrary.h"
#include "SimpliCityObjectSelector.h"
#include "SimpliCityObjectManager.h"
#include "Zone/SimpliCityZoneCell.h"
#include "Building/SimpliCityBuildingBase.h"

using SCFL = USimpliCityFunctionLibrary;

ASimpliCityZoneManager::ASimpliCityZoneManager() {
  DefaultBlueprintClass = ASimpliCityZoneBase::StaticClass();
}

void ASimpliCityZoneManager::BeginPlay() {
  Super::BeginPlay();
  InitializeCellZones();
  ObjectSelector = SCFL::GetSelector(this);
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityZoneManager::Enable(UTexture2D* NewIcon) {
  ASimpliCityBaseManager::Enable(NewIcon);
  GetBuildType(NewIcon, BuildType);
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityZoneManager::Disable() {
  ASimpliCityBaseManager::Disable();
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityZoneManager::StartBuilding() {
  ASimpliCityBaseManager::StartBuilding();
  ObjectSelector->VisualizeSelection = false;
}

UMaterialInstance* ASimpliCityZoneManager::GetTypeMaterial(ESimpliCityZoneType ZoneType) {
  switch (ZoneType) {
  case ESimpliCityZoneType::ZoneType_Residential:
    return ResidentialMaterial;
  case ESimpliCityZoneType::ZoneType_Commercial:
    return CommercialMaterial;
  case ESimpliCityZoneType::ZoneType_Industrial:
    return IndustrialMaterial;
  default: // none
    return DefaultMaterial;
  }
}

//////////////////////////////////////////////////////////////////////////
bool ASimpliCityZoneManager::UpdateBuilding(FVector Location) {
  ASimpliCityBaseManager::UpdateBuilding(Location);
  TSet<ASimpliCityZoneBase*> newCells =
      TSet<ASimpliCityZoneBase*>(ObjectSelector->UpdateSelection<ASimpliCityZoneBase>(StartLocation, LastLocation));
  if (newCells.Num() == 0) {
    ReloadAllCellStates();
    return false;
  }
  TSet<ASimpliCityZoneBase*> reloadCells = oldZones.Difference(newCells);
  // reload states of cells that aren't in the selection area anymore
  for (auto const& cell : reloadCells.Array()) {
    ReloadCellState(cell);
  }
  TSet<ASimpliCityZoneBase*> saveCells = newCells.Difference(oldZones);
  for (auto const& cell : saveCells.Array()) {
    SaveLastCellState(cell);
    cell->SetZoneType(BuildType, GetTypeMaterial(BuildType));
  }
  oldZones = newCells;
  return true;
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityZoneManager::FinishBuilding() {
  ASimpliCityBaseManager::FinishBuilding();
  ObjectSelector->FinishSelection();
  ResetCellStates();
  for (auto Zone : LastZoneStateMap) {
    // if this zone's type differs from previous
    auto CurrentType = Zone.Key->GetZoneType();
    auto LastType = Zone.Value;
    if (CurrentType != LastType) {
      TArray<ASimpliCityZoneBase*>& List = ZonesPerType.FindOrAdd(CurrentType);
      List.Add(Zone.Key);
      //ZonesPerType[CurrentType] = List;
      List = ZonesPerType.FindOrAdd(LastType);
      List.Remove(Zone.Key);
      //ZonesPerType[LastType] = List;
    }
  }
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityZoneManager::CancelBuilding() {
  ASimpliCityBaseManager::CancelBuilding();
  ObjectSelector->FinishSelection();
  ReloadAllCellStates();
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityZoneManager::InitializeCellZones() {
  int numRows = GridManager->GetNumRows();
  int numCols = GridManager->GetNumCols();
  for (int row = 0; row < numRows; row++) {
    for (int col = 0; col < numCols; col++) {
      FVector location = GridManager->TileToLocation(row, col);
      location += FVector(0, 0, 1);
      ASimpliCityZoneBase* Zone = GetWorld()->SpawnActor<ASimpliCityZoneBase>(DefaultBlueprintClass, location, FRotator());
      FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepWorld, false);
      Zone->AttachToActor(this, AttachmentRules);
      ZoneGrid.Add(Zone);
    }
  }
}

//////////////////////////////////////////////////////////////////////////
// When currently selecting zones, store the last state of each cell before applying new zone
// if selection is canceled, load last state for each cell (reload)
bool ASimpliCityZoneManager::SaveLastCellState(ASimpliCityZoneBase* Cell) {
  if (LastZoneStateMap.Contains(Cell) == false) {
    LastZoneStateMap.Add(Cell, Cell->GetZoneType());
    return true;
  }
  return false;
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityZoneManager::ReloadCellState(ASimpliCityZoneBase* Cell) {
  if (LastZoneStateMap.Contains(Cell) == true) {
    auto Type = LastZoneStateMap[Cell];
    Cell->SetZoneType(Type, GetTypeMaterial(Type));
    LastZoneStateMap.Remove(Cell);
  }
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityZoneManager::ReloadAllCellStates() {
  for (auto Cell : LastZoneStateMap) {
    auto Type = Cell.Value;
    Cell.Key->SetZoneType(Type, GetTypeMaterial(Type));
  }
  ResetCellStates();
}

//////////////////////////////////////////////////////////////////////////
// clear all saved states
void ASimpliCityZoneManager::ResetCellStates() {
  LastZoneStateMap.Empty();
}

//////////////////////////////////////////////////////////////////////////
TEnumAsByte<ESimpliCityZoneType> ASimpliCityZoneManager::GetZoneTypeAtLocation(FVector Location) {
  int32 index = GridManager->LocationToIndex(Location);
  if (index <= 0 || index >= ZoneGrid.Num()) {
    TRACE_ERROR_PRINTF(LogSimpliCity, "ERROR!! NOT IN RANGE!! (0 <= index(%d) < ZonedZoneGrid.Num()(%d)", index,
                       ZoneGrid.Num());
    return ESimpliCityZoneType::ZoneType_None;
  }
  if (ZoneGrid[index] == nullptr) {
    TRACE_ERROR_PRINTF(LogSimpliCity, "ERROR!!! ZonedZoneGrid[index] == nullptr");
    return ESimpliCityZoneType::ZoneType_None;
  }
  return ZoneGrid[index]->GetZoneType();
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityZoneManager::DestroyObjects(const TArray<ASimpliCityObjectBase*>& ObjectList) {
  for (auto Object : ObjectList) {
    if (Object == nullptr)
      continue;
    if (ASimpliCityBuildingBase* Building = Cast<ASimpliCityBuildingBase>(Object)) {
      RemoveBuildingFromList(Building);
      ObjectManager->RemoveObjectFromGrid(Object);
    }
  }
}

//////////////////////////////////////////////////////////////////////////
TArray<ASimpliCityBuildingBase*> ASimpliCityZoneManager::GetAllBuildingsOfType(ESimpliCityZoneType Type) {
  TArray<ASimpliCityBuildingBase*> BuildingList;
  TArray<ASimpliCityZoneBase*> ZonesOfType = ZonesPerType[Type];
  for (auto Zone : ZonesOfType) {
    ASimpliCityBuildingBase* const* Building = BuildingToZoneMap.FindKey(Zone);
    if (Building != nullptr) {
      BuildingList.Add(*Building);
    }
  }
  return BuildingList;
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityZoneManager::AddBuildingToList(ASimpliCityZoneBase* Zone, ASimpliCityBuildingBase* Building) {
  if (Zone == nullptr || Building == nullptr)
    return;
  BuildingToZoneMap.Emplace(Building, Zone);
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityZoneManager::RemoveBuildingFromList(ASimpliCityBuildingBase* Building) {
  if (Building == nullptr)
    return;
  BuildingToZoneMap.Remove(Building);
}

TArray<ASimpliCityZoneBase*> ASimpliCityZoneManager::GetEmptyZones() {
  TArray<ASimpliCityZoneBase*> EmptyZones;
  for (auto Zone : ZoneGrid) {
    bool Exists = ObjectManager->DoesObjectExistHere(Zone->GetActorLocation());
    if (Exists == false) {
      EmptyZones.Add(Zone);
    }
  }
  return EmptyZones;
}