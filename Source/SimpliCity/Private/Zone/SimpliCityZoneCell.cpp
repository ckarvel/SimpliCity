// Fill out your copyright notice in the Description page of Project Settings.


#include "Zone/SimpliCityZoneCell.h"
#include "Components/StaticMeshComponent.h"
#include "Zone/SimpliCityZoneManager.h"
#include "SimpliCityFunctionLibrary.h"

// Sets default values
ASimpliCityZoneCell::ASimpliCityZoneCell()
	: ZoneType(ESimpliCityZoneTypeEnum::ZoneType_None) {
	PrimaryActorTick.bCanEverTick = false;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMeshComponent);
}

// Called when the game starts or when spawned
void ASimpliCityZoneCell::BeginPlay() {
	Super::BeginPlay();
	if (DefaultMaterial == nullptr) {
		TRACE_ERROR_PRINTF(LogSimpliCity,"ERROR!!! DefaultMaterial == nullptr");
		return;
	}
	if (ResidentialMaterial == nullptr) {
		TRACE_ERROR_PRINTF(LogSimpliCity,"ERROR!!! ResidentialMaterial == nullptr");
		return;
	}
	if (CommercialMaterial == nullptr) {
		TRACE_ERROR_PRINTF(LogSimpliCity,"ERROR!!! CommercialMaterial == nullptr");
		return;
	}
	if (IndustrialMaterial == nullptr) {
		TRACE_ERROR_PRINTF(LogSimpliCity,"ERROR!!! IndustrialMaterial == nullptr");
		return;
	}
}

void ASimpliCityZoneCell::SetCellType(TEnumAsByte<ESimpliCityZoneTypeEnum> Type) {
	if (Type != ZoneType) {
		ZoneType = Type;
		UpdateActiveMaterial();
	}
}

void ASimpliCityZoneCell::UpdateActiveMaterial() {
	switch (ZoneType) {
		case ESimpliCityZoneTypeEnum::ZoneType_Residential:
			StaticMeshComponent->SetMaterial(0,ResidentialMaterial);
			break;
		case ESimpliCityZoneTypeEnum::ZoneType_Commercial:
			StaticMeshComponent->SetMaterial(0,CommercialMaterial);
			break;
		case ESimpliCityZoneTypeEnum::ZoneType_Industrial:
			StaticMeshComponent->SetMaterial(0,IndustrialMaterial);
			break;
		default: // none
			StaticMeshComponent->SetMaterial(0,DefaultMaterial);
	}
}
