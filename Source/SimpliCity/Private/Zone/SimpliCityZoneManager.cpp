// Fill out your copyright notice in the Description page of Project Settings.

#include "Zone/SimpliCityZoneManager.h"

#include "GridManager.h"
#include "SimpliCityFunctionLibrary.h"
#include "SimpliCityObjectSelector.h"
#include "SimpliCityObjectManager.h"
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
void ASimpliCityZoneManager::Enable(ESimpliCityResourceType _TypeId) {
  ASimpliCityBaseManager::Enable(_TypeId);

  if (BuildIconToType.Contains(_TypeId) == false) {
    ESimpliCityZoneType BuildType;
    GetBuildType(_TypeId, BuildType);
    BuildIconToType.Emplace(_TypeId, BuildType);
  }

  GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityZoneManager::Disable() {
  ASimpliCityBaseManager::Disable();
  GetWorldTimerManager().SetTimer(SpawnTimerHandle, this,
                                  &ASimpliCityZoneManager::TrySpawningBuildings,
                                  2, true);
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityZoneManager::StartBuilding() {
  ASimpliCityBaseManager::StartBuilding();
  ObjectSelector->VisualizeSelection = false;
}

UMaterialInstance* ASimpliCityZoneManager::GetTypeMaterial(ESimpliCityZoneType ZoneType) {
  switch (ZoneType) {
  case ESimpliCityZoneType::Residential:
    return ResidentialMaterial;
  case ESimpliCityZoneType::Commercial:
    return CommercialMaterial;
  case ESimpliCityZoneType::Industrial:
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
    ESimpliCityZoneType BuildType = BuildIconToType[ResourceType];
    cell->SetZoneType(ResourceType, BuildType, GetTypeMaterial(BuildType));
  }
  oldZones = newCells;
  return true;
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityZoneManager::FinishBuilding() {
  // HACK- Add cells to temp list so base manager
  // can detect if build error
  // UPDATE - for now don't check zone cost
  //TArray<ASimpliCityZoneBase*> Cells;
  // LastZoneStateMap.GenerateKeyArray(Cells);
  // for (auto Cell : Cells) {
  //  Temporary_ObjectToLocation.Add(FVector(), Cell);
  //}
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
    LastZoneStateMap.Add(Cell, Cell->ResourceType);
    return true;
  }
  return false;
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityZoneManager::ReloadCellState(ASimpliCityZoneBase* Cell) {
  if (LastZoneStateMap.Contains(Cell) == true) {
    ESimpliCityZoneType BuildType = ESimpliCityZoneType::None;
    ESimpliCityResourceType TypeId = LastZoneStateMap[Cell];
    if (!BuildIconToType.Contains(TypeId)) {
      TRACE_ERROR_PRINTF(LogSimpliCity, "ERROR!!ASimpliCityZoneManager::ReloadCellState, Type in LastZoneStateMap \
but not in BuildIconToType");
    } else {
      BuildType = BuildIconToType[TypeId];
    }
    Cell->SetZoneType(TypeId, BuildType, GetTypeMaterial(BuildType));
    LastZoneStateMap.Remove(Cell);
  }
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityZoneManager::ReloadAllCellStates() {
  for (auto Cell : LastZoneStateMap) {
    ESimpliCityZoneType BuildType = ESimpliCityZoneType::None;
    ESimpliCityResourceType TypeId = Cell.Value;
    if (!BuildIconToType.Contains(TypeId)) {
      TRACE_ERROR_PRINTF(LogSimpliCity, "ERROR!!ASimpliCityZoneManager::ReloadCellState, Type in LastZoneStateMap \
but not in BuildIconToType");
    } else {
      BuildType = BuildIconToType[TypeId];
    }
    Cell.Key->SetZoneType(TypeId, BuildType, GetTypeMaterial(BuildType));
  }
  ResetCellStates();
}

//////////////////////////////////////////////////////////////////////////
// clear all saved states
void ASimpliCityZoneManager::ResetCellStates() {
  LastZoneStateMap.Empty();
}

//////////////////////////////////////////////////////////////////////////
ESimpliCityZoneType ASimpliCityZoneManager::GetZoneTypeAtLocation(FVector Location) {
  int32 index = GridManager->LocationToIndex(Location);
  if (index <= 0 || index >= ZoneGrid.Num()) {
    TRACE_ERROR_PRINTF(LogSimpliCity, "ERROR!! NOT IN RANGE!! (0 <= index(%d) < ZonedZoneGrid.Num()(%d)", index,
                       ZoneGrid.Num());
    return ESimpliCityZoneType::None;
  }
  if (ZoneGrid[index] == nullptr) {
    TRACE_ERROR_PRINTF(LogSimpliCity, "ERROR!!! ZonedZoneGrid[index] == nullptr");
    return ESimpliCityZoneType::None;
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

//////////////////////////////////////////////////////////////////////////
TArray<ASimpliCityZoneBase*> ASimpliCityZoneManager::GetEmptyZones() {
  TArray<ASimpliCityZoneBase*> EmptyZones;
  for (auto Zone : ZoneGrid) {
    // looking for zones that have a valid type and no building spawned
    if (Zone->GetZoneType() == ESimpliCityZoneType::None)
      continue;
    bool Exists = ObjectManager->DoesObjectExistHere(Zone->GetActorLocation());
    if (Exists == false) {
      EmptyZones.Add(Zone);
    }
  }
  return EmptyZones;
}

//////////////////////////////////////////////////////////////////////////
void ASimpliCityZoneManager::TrySpawningBuildings() {
  TArray<ASimpliCityZoneBase*> EmptyZones = GetEmptyZones();
  for (const auto& Zone : EmptyZones) {
    // near a road?
    TArray<ASimpliCityObjectBase*> Roads = ObjectManager->GetNeighborsOfType(Zone->GetActorLocation(),
                                                                       ESimpliCityObjectType::Road);
    if (Roads.Num() > 0) {
      // valid
      if (auto* BuildingObject = SpawnObjectOfType(DefaultBlueprintClass,
          FVector(0, 0, -1000), FRotator(0, 0, 0), Zone->ResourceType)) {
        #if WITH_EDITOR
                BuildingObject->SetFolderPath("Buildings");
        #endif

        BuildingObject->SetNewLocation(Zone->GetActorLocation());
        ObjectManager->AddObjectToGrid(BuildingObject);
        BuildingObject->OnObjectPlaced();
        break;
      }
    }
  }
}