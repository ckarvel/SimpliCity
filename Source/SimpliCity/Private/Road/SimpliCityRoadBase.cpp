// Fill out your copyright notice in the Description page of Project Settings.


#include "Road/SimpliCityRoadBase.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ASimpliCityRoadBase::ASimpliCityRoadBase()
{
	PrimaryActorTick.bCanEverTick = false;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RoadMesh"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	// TEnumAsByte<ESimpliCityBuildObjectEnum> BuildType;
	// BuildObject_None UMETA(DisplayName = "None"),
	// BuildObject_Road UMETA(DisplayName = "Road")
	BuildType = ESimpliCityBuildObjectEnum::BuildObject_Road;
}

// Called when the game starts or when spawned
void ASimpliCityRoadBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called when the game starts or when spawned
void ASimpliCityRoadBase::SetNewMaterial() {
	check(StaticMeshComponent != nullptr);
	if (RoadMaterial != nullptr) {
		StaticMeshComponent->SetMaterial(0, RoadMaterial);
	}
}