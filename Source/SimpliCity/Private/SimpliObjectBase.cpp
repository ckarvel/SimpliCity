// Fill out your copyright notice in the Description page of Project Settings.

#include "SimpliObjectBase.h"

#include "SimpliCityFunctionLibrary.h"
#include "SimpliCityObjectBase.h"
#include "SimpliCityObjectManager.h"

using SCFL = USimpliCityFunctionLibrary;

ASimpliCityObjectBase* USimpliObjectBase::SpawnActor(FVector Location) {
  SpawnedActor = GetOuter()->GetWorld()->SpawnActor<ASimpliCityObjectBase>(DefaultClass, Location, FRotator());
  SCFL::GetObjectManager(this)->AddObjectToGrid(SpawnedActor);
  SpawnedActor->SetObjectBase(this);
  return SpawnedActor;
}

void USimpliObjectBase::DestroyActor() {
  if (SpawnedActor == nullptr)
    return;
  SCFL::GetObjectManager(this)->RemoveObjectFromGrid(SpawnedActor);
  SpawnedActor->Destroy();
  SpawnedActor = nullptr;
}
