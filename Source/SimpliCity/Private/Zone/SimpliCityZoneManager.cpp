// Fill out your copyright notice in the Description page of Project Settings.


#include "Zone/SimpliCityZoneManager.h"
#include "Zone/SimpliCityZoneCell.h"
#include "SimpliCityFunctionLibrary.h"
#include "GridManager.h"

ASimpliCityZoneManager::ASimpliCityZoneManager()
{
	PrimaryActorTick.bCanEverTick = true;
	ZoneCellClass = ASimpliCityZoneCell::StaticClass();
}

void ASimpliCityZoneManager::BeginPlay()
{
	Super::BeginPlay();
	InitializeCellZones();
}

void ASimpliCityZoneManager::InitializeCellZones() {
	AGridManager* gridMgr = USimpliCityFunctionLibrary::GetGridManager(this);
	int numRows = gridMgr->GetNumRows();
  int numCols = gridMgr->GetNumCols();
	for (int row = 0; row < numRows; row++) {
		for (int col = 0; col < numCols; col++) {
			FVector location = gridMgr->TileToLocation(row,col);
			location += FVector(0,0,1);
			ASimpliCityZoneCell* Cell = GetWorld()->SpawnActor<ASimpliCityZoneCell>(ZoneCellClass,location, FRotator());
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepWorld, false);
			Cell->AttachToActor(this,AttachmentRules);
			ZonedGridCells.Add(Cell);
		}
	}
}

bool ASimpliCityZoneManager::IsCellSelected(ASimpliCityZoneCell* Cell) {
	return TemporaryCellStates.Contains(Cell);
}

bool ASimpliCityZoneManager::BackupCellDataIfNotExists(ASimpliCityZoneCell* Cell) {
	if (IsCellSelected(Cell) == false) {
		TemporaryCellStates.Add(Cell, Cell->ZoneType);
		return true;
	}
	return false;
}

void ASimpliCityZoneManager::RestoreCellData(ASimpliCityZoneCell* Cell) {
	if (IsCellSelected(Cell) == true) {
		Cell->SetCellType(TemporaryCellStates[Cell]);
		TemporaryCellStates.Remove(Cell);
	}
}

void ASimpliCityZoneManager::RestoreAllCellData() {
	for (auto Cell : TemporaryCellStates) {
		Cell.Key->SetCellType(Cell.Value);
	}
	TemporaryCellStates.Empty();
}

void ASimpliCityZoneManager::ClearBackupData() {
	TemporaryCellStates.Empty();
}

TArray<ASimpliCityZoneCell*> ASimpliCityZoneManager::GetUnfilledZonedCells() {
	return EmptyZones;
}

void ASimpliCityZoneManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

TEnumAsByte<ESimpliCityZoneTypeEnum> ASimpliCityZoneManager::GetZoneTypeAtLocation(FVector Location) {
	int32 index = USimpliCityFunctionLibrary::GetGridManager(this)->LocationToIndex(Location);
	check(index >= 0);
	check(index < ZonedGridCells.Num());
	check(ZonedGridCells[index] != nullptr);
	return ZonedGridCells[index]->ZoneType;
}