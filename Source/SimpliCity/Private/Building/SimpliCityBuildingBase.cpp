// Fill out your copyright notice in the Description page of Project Settings.


#include "Building/SimpliCityBuildingBase.h"
#include "Components/StaticMeshComponent.h"

#include "Road/SimpliCityRoadBase.h"

#include "MarkerComponent.h"
#include "MarkerManager.h"

#include "Tools/SimpliCityFunctionLibrary.h"

ASimpliCityBuildingBase::ASimpliCityBuildingBase()
{
	PrimaryActorTick.bCanEverTick = false;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildingMesh"));
	SetRootComponent(StaticMeshComponent);
	BuildType = ESimpliCityBuildObjectEnum::BuildObject_Building;

}

void ASimpliCityBuildingBase::BeginPlay()
{
	Super::BeginPlay();
	
}
