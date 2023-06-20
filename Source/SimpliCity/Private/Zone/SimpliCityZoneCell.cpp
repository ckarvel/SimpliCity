// Fill out your copyright notice in the Description page of Project Settings.

#include "Zone/SimpliCityZoneCell.h"

#include "Components/StaticMeshComponent.h"
#include "SimpliCityFunctionLibrary.h"
#include "Zone/SimpliCityZoneManager.h"

// Sets default values
ASimpliCityZoneCell::ASimpliCityZoneCell() : ZoneType(ESimpliCityZoneType::ZoneType_None) {
  PrimaryActorTick.bCanEverTick = false;
  StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
  SetRootComponent(StaticMeshComponent);
}

// Called when the game starts or when spawned
void ASimpliCityZoneCell::BeginPlay() {
  Super::BeginPlay();
}

void ASimpliCityZoneCell::SetCellType(TEnumAsByte<ESimpliCityZoneType> Type) {
  if (Type != ZoneType) {
    ZoneType = Type;
    UpdateActiveMaterial();
  }
}

void ASimpliCityZoneCell::UpdateActiveMaterial() {
}
