// Fill out your copyright notice in the Description page of Project Settings.

#include "SimpliCityObjectBase.h"

#include "SimpliCityFunctionLibrary.h"
#include "SimpliCityObjectManager.h"
#include "GridManager.h"

using SCFL = USimpliCityFunctionLibrary;

// Sets default values
ASimpliCityObjectBase::ASimpliCityObjectBase()
    : bIsValidPlacement(false) {
  PrimaryActorTick.bCanEverTick = false;
  StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObjectMesh"));
  SetRootComponent(StaticMeshComponent);
}

void ASimpliCityObjectBase::BeginPlay() {
  Super::BeginPlay();
  GridManager = SCFL::GetGridManager(this);
  //ObjectManager = SCFL::GetObjectManager(this);
}

void ASimpliCityObjectBase::SetNewLocation(FVector Location) {
  SetActorLocation(Location);
  //bIsValidPlacement = ObjectManager->DoesObjectExistHere(Location) == false;
}

void ASimpliCityObjectBase::SetPlacementValid(bool IsValid) {
  bIsValidPlacement = IsValid;
  OnValidStateChanged(bIsValidPlacement);
}
