// Fill out your copyright notice in the Description page of Project Settings.

#include "Zone/SimpliCityZoneManager.h"

#include "GridManager.h"
#include "SimpliCityFunctionLibrary.h"
#include "SimpliCityObjectManager.h"
#include "SimpliCityObjectSelector.h"
#include "Zone/SimpliCityZoneCell.h"

using SCFL = USimpliCityFunctionLibrary;

ASimpliCityZoneManager::ASimpliCityZoneManager() {
  ZoneCellClass = ASimpliCityZoneCell::StaticClass();
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

//////////////////////////////////////////////////////////////////////////
bool ASimpliCityZoneManager::UpdateBuilding(FVector Location) {
  ASimpliCityBaseManager::UpdateBuilding(Location);
  TSet<ASimpliCityZoneCell*> newCells =
      TSet<ASimpliCityZoneCell*>(ObjectSelector->UpdateSelection<ASimpliCityZoneCell>(StartLocation, LastLocation));
  if (newCells.Num() == 0) {
    ReloadAllCellStates();
    return false;
  }
  TSet<ASimpliCityZoneCell*> reloadCells = oldCells.Difference(newCells);
  // reload states of cells that aren't in the selection area anymore
  for (auto const& cell : reloadCells.Array()) {
    ReloadCellState(cell);
  }
  TSet<ASimpliCityZoneCell*> saveCells = newCells.Difference(oldCells);
  for (auto const& cell : saveCells.Array()) {
    SaveLastCellState(cell);
    cell->SetCellType(BuildType);
  }
  oldCells = newCells;
  return true;
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityZoneManager::FinishBuilding() {
  ASimpliCityBaseManager::FinishBuilding();
  ObjectSelector->FinishSelection();
  ResetCellStates();
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityZoneManager::CancelBuilding() {
  ASimpliCityBaseManager::CancelBuilding();
  ObjectSelector->FinishSelection();
  ReloadAllCellStates();
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityZoneManager::InitializeCellZones() {
  AGridManager* gridMgr = USimpliCityFunctionLibrary::GetGridManager(this);
  int numRows = gridMgr->GetNumRows();
  int numCols = gridMgr->GetNumCols();
  for (int row = 0; row < numRows; row++) {
    for (int col = 0; col < numCols; col++) {
      FVector location = gridMgr->TileToLocation(row, col);
      location += FVector(0, 0, 1);
      ASimpliCityZoneCell* Cell = GetWorld()->SpawnActor<ASimpliCityZoneCell>(ZoneCellClass, location, FRotator());
      FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepWorld, false);
      Cell->AttachToActor(this, AttachmentRules);
      GridCells.Add(Cell);
    }
  }
}

//////////////////////////////////////////////////////////////////////////
// When currently selecting zones, store the last state of each cell before applying new zone
// if selection is canceled, load last state for each cell (reload)
bool ASimpliCityZoneManager::SaveLastCellState(ASimpliCityZoneCell* Cell) {
  if (LastCellStateMap.Contains(Cell) == false) {
    LastCellStateMap.Add(Cell, Cell->ZoneType);
    return true;
  }
  return false;
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityZoneManager::ReloadCellState(ASimpliCityZoneCell* Cell) {
  if (LastCellStateMap.Contains(Cell) == true) {
    Cell->SetCellType(LastCellStateMap[Cell]);
    LastCellStateMap.Remove(Cell);
  }
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityZoneManager::ReloadAllCellStates() {
  for (auto Cell : LastCellStateMap) {
    Cell.Key->SetCellType(Cell.Value);
  }
  ResetCellStates();
}

//////////////////////////////////////////////////////////////////////////
// clear all saved states
void ASimpliCityZoneManager::ResetCellStates() {
  LastCellStateMap.Empty();
}

//////////////////////////////////////////////////////////////////////////
TEnumAsByte<ESimpliCityZoneType> ASimpliCityZoneManager::GetZoneTypeAtLocation(FVector Location) {
  int32 index = USimpliCityFunctionLibrary::GetGridManager(this)->LocationToIndex(Location);
  if (index <= 0 || index >= GridCells.Num()) {
    TRACE_ERROR_PRINTF(LogSimpliCity, "ERROR!! NOT IN RANGE!! (0 <= index(%d) < ZonedGridCells.Num()(%d)", index,
                       GridCells.Num());
    return ESimpliCityZoneType::ZoneType_None;
  }
  if (GridCells[index] == nullptr) {
    TRACE_ERROR_PRINTF(LogSimpliCity, "ERROR!!! ZonedGridCells[index] == nullptr");
    return ESimpliCityZoneType::ZoneType_None;
  }
  return GridCells[index]->ZoneType;
}

//////////////////////////////////////////////////////////////////////////
bool ASimpliCityZoneManager::PlacePermanentZoneBase(ASimpliCityZoneBase* ZoneBase) {
  if (ZoneBase == nullptr)
    return false;
  if (SCFL::GetObjectManager(this)->DoesObjectExistHere(ZoneBase->GetActorLocation())) {
    return false;
  }
  SCFL::GetObjectManager(this)->AddObjectToGrid(ZoneBase);
  AddZoneBaseToList(ZoneBase->ZoneType, ZoneBase);
  return true;
}

ASimpliCityObjectBase* ASimpliCityZoneManager::PlacePermanentObject(TSubclassOf<ASimpliCityObjectBase> ObjectClass,
                                                                        const FVector Location,
                                                                        const FRotator Rotation) {
  ASimpliCityObjectBase* Object = ASimpliCityBaseManager::PlacePermanentObject(ObjectClass, Location, Rotation);
  if (Object == nullptr) {
    return Object;
  }
  ASimpliCityZoneBase* Building = Cast<ASimpliCityZoneBase>(Object);
  AddZoneBaseToList(Building->ZoneType, Building);
  return Object;
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityZoneManager::DestroyObjects(const TArray<ASimpliCityObjectBase*>& ObjectList) {
  for (auto Object : ObjectList) {
    if (Object == nullptr)
      continue;
    if (ASimpliCityZoneBase* ZoneBase = Cast<ASimpliCityZoneBase>(Object)) {
      RemoveZoneBaseFromList(ZoneBase);
      SCFL::GetObjectManager(this)->RemoveObjectFromGrid(ZoneBase);
    }
  }
}

//////////////////////////////////////////////////////////////////////////
TArray<ASimpliCityZoneBase*> ASimpliCityZoneManager::GetAllZoneBasesOfType(ESimpliCityZoneType Type) {
  TArray<ASimpliCityZoneBase*> ZoneBaseList = ZoneBasesPerType.FindOrAdd(Type);
  return ZoneBaseList;
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityZoneManager::AddZoneBaseToList(ESimpliCityZoneType Type, ASimpliCityZoneBase* ZoneBase) {
  if (Type == ESimpliCityZoneType::ZoneType_None)
    return;
  TArray<ASimpliCityZoneBase*> ZoneBaseList = ZoneBasesPerType.FindOrAdd(Type);
  if (ZoneBaseList.Contains(ZoneBase) == false) {
    ZoneBaseList.Add(ZoneBase);
    ZoneBasesPerType[Type] = ZoneBaseList;
  }
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityZoneManager::RemoveZoneBaseFromList(ASimpliCityZoneBase* ZoneBase) {
  if (ZoneBase == nullptr)
    return;
  ESimpliCityZoneType Type = ZoneBase->ZoneType;
  TArray<ASimpliCityZoneBase*> ZoneBaseList = ZoneBasesPerType.FindOrAdd(Type);
  if (ZoneBaseList.Contains(ZoneBase)) {
    ZoneBaseList.Remove(ZoneBase);
    ZoneBasesPerType[Type] = ZoneBaseList;
  }
}
