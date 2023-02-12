// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SimpliCity.h"
#include "GameFramework/Actor.h"
#include "Zone/SimpliCityZoneTypeEnum.h"
#include "SimpliCityZoneCell.generated.h"

UCLASS(Blueprintable)
class SIMPLICITY_API ASimpliCityZoneCell : public AActor {
	GENERATED_BODY()

public:
	ASimpliCityZoneCell();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable,Category = "SimpliCityZoneCell")
	void SetCellType(TEnumAsByte<ESimpliCityZoneTypeEnum> Type);

	UFUNCTION(BlueprintCallable,Category = "SimpliCityZoneCell")
	void UpdateActiveMaterial();

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "SimpliCityZoneCell")
	TEnumAsByte<ESimpliCityZoneTypeEnum> ZoneType;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = "SimpliCityZoneCell")
	class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "SimpliCityZoneCell")
	class UMaterialInstance* DefaultMaterial;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "SimpliCityZoneCell")
	class UMaterialInstance* ResidentialMaterial;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "SimpliCityZoneCell")
	class UMaterialInstance* CommercialMaterial;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "SimpliCityZoneCell")
	class UMaterialInstance* IndustrialMaterial;
};
