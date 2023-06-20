// Fill out your copyright notice in the Description page of Project Settings.

#include "SimpliCityObjectBase.h"

#include "SimpliCityFunctionLibrary.h"
#include "SimpliCityObjectManager.h"

using SCFL = USimpliCityFunctionLibrary;

// Sets default values
ASimpliCityObjectBase::ASimpliCityObjectBase() {
  PrimaryActorTick.bCanEverTick = false;
  StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObjectMesh"));
  SetRootComponent(StaticMeshComponent);
}

void ASimpliCityObjectBase::BeginPlay() {
  Super::BeginPlay();
}

// Called when the game starts or when spawned
void ASimpliCityObjectBase::SetNewMaterial(UMaterialInstance* Material) {
  if (StaticMeshComponent == nullptr || Material == nullptr) {
    TRACE_ERROR_PRINTF(LogSimpliCity, "ERROR!!! StaticMeshComponent or Material == nullptr");
    return;
  }
  StaticMeshComponent->SetMaterial(0, Material);
}

void ASimpliCityObjectBase::SetNewLocation(FVector Location) {
  SetActorLocation(Location);
}

// Called when object is placed down
void ASimpliCityObjectBase::OnObjectPlaced() {
  // implement
}
