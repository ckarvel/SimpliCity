// Fill out your copyright notice in the Description page of Project Settings.


#include "Zone/SimpliCityZoneManager.h"
#include "Zone/SimpliCityZoneCell.h"

#include "Tools/SimpliCityFunctionLibrary.h"
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

void ASimpliCityZoneManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
