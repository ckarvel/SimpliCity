// Fill out your copyright notice in the Description page of Project Settings.


#include "Zone/SimpliCityZoneBase.h"
#include "Components/StaticMeshComponent.h"

#include "Road/SimpliCityRoadBase.h"

#include "MarkerComponent.h"
#include "MarkerManager.h"

#include "SimpliCityFunctionLibrary.h"

// Sets default values
ASimpliCityZoneBase::ASimpliCityZoneBase()
{
	PrimaryActorTick.bCanEverTick = false;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ZoneMesh"));
	SetRootComponent(StaticMeshComponent);
	ObjectType = ESimpliCityObjectType::Zone;
}

// Called when the game starts or when spawned
void ASimpliCityZoneBase::BeginPlay()
{
	Super::BeginPlay();
	
}
