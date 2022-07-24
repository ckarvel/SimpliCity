// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpliCityRoadBase.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ASimpliCityRoadBase::ASimpliCityRoadBase()
{
	PrimaryActorTick.bCanEverTick = false;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RoadMesh"));
	StaticMeshComponent->SetupAttachment(RootComponent);
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