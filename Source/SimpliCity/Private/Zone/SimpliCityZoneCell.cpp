// Fill out your copyright notice in the Description page of Project Settings.

#include "Zone/SimpliCityZoneCell.h"
#include "Components/StaticMeshComponent.h"
#include "SimpliCityFunctionLibrary.h"
#include "Zone/SimpliCityZoneManager.h"

// Sets default values
ASimpliCityZoneCell::ASimpliCityZoneCell()
  : ZoneType(ESimpliCityZoneType::ZoneType_None) {
  PrimaryActorTick.bCanEverTick = false;
  StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
  SetRootComponent(StaticMeshComponent);
}

// Called when the game starts or when spawned
void ASimpliCityZoneCell::BeginPlay() {
  Super::BeginPlay();
  if (DefaultMaterial == nullptr) {
    TRACE_ERROR_PRINTF(LogSimpliCity, "ERROR!!! DefaultMaterial == nullptr");
    return;
  }
  if (ResidentialMaterial == nullptr) {
    TRACE_ERROR_PRINTF(LogSimpliCity, "ERROR!!! ResidentialMaterial == nullptr");
    return;
  }
  if (CommercialMaterial == nullptr) {
    TRACE_ERROR_PRINTF(LogSimpliCity, "ERROR!!! CommercialMaterial == nullptr");
    return;
  }
  if (IndustrialMaterial == nullptr) {
    TRACE_ERROR_PRINTF(LogSimpliCity, "ERROR!!! IndustrialMaterial == nullptr");
    return;
  }
}

void ASimpliCityZoneCell::SetCellType(TEnumAsByte<ESimpliCityZoneType> Type) {
  if (Type != ZoneType) {
    ZoneType = Type;
    UpdateActiveMaterial();
  }
}

void ASimpliCityZoneCell::UpdateActiveMaterial() {
  switch (ZoneType) {
  case ESimpliCityZoneType::ZoneType_Residential:
    StaticMeshComponent->SetMaterial(0, ResidentialMaterial);
    break;
  case ESimpliCityZoneType::ZoneType_Commercial:
    StaticMeshComponent->SetMaterial(0, CommercialMaterial);
    break;
  case ESimpliCityZoneType::ZoneType_Industrial:
    StaticMeshComponent->SetMaterial(0, IndustrialMaterial);
    break;
  default: // none
    StaticMeshComponent->SetMaterial(0, DefaultMaterial);
  }
}
