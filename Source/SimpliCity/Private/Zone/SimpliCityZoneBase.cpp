// Fill out your copyright notice in the Description page of Project Settings.

#include "Zone/SimpliCityZoneBase.h"

#include "Components/StaticMeshComponent.h"
#include "MarkerComponent.h"
#include "MarkerManager.h"
#include "Road/SimpliCityRoadBase.h"
#include "SimpliCityFunctionLibrary.h"

// Sets default values
ASimpliCityZoneBase::ASimpliCityZoneBase() {
  //ObjectType = ESimpliCityObjectType::Zone;
}

void ASimpliCityZoneBase::SetZoneType(ESimpliCityResourceType TypeId,
                                      ESimpliCityZoneType Type,
                                      UMaterialInstance* Material) {
  ResourceType = TypeId;

  if (Type != ZoneType) {
    ZoneType = Type;
  }
  if (ActiveMaterial != Material) {
    ActiveMaterial = Material;
  }
  ShowActiveMaterial();
}

void ASimpliCityZoneBase::ShowActiveMaterial() {
  StaticMeshComponent->SetMaterial(0, ActiveMaterial);
}
