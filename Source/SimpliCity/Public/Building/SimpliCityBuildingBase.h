// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Builder/SimpliCityBuildObjectBase.h"
#include "Building/SimpliCityBuildingEnum.h"
#include "SimpliCityBuildingBase.generated.h"

UCLASS(Blueprintable)
class SIMPLICITY_API ASimpliCityBuildingBase : public ASimpliCityBuildObjectBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASimpliCityBuildingBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintCallable,Category="SimpliCityBuildingBase")
	FORCEINLINE class UStaticMeshComponent* GetStaticMeshComponent() const { return StaticMeshComponent; }

	UFUNCTION(BlueprintCallable,Category="SimpliCityBuildingBase")
	class ASimpliCityRoadBase* GetRoadConnection() const { return RoadConnection; }

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="SimpliCityBuildingBase")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="SimpliCityBuildingBase")
	ASimpliCityRoadBase* RoadConnection;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="SimpliCityBuildingBase",Meta = (ExposeOnSpawn=true))
	TEnumAsByte<ESimpliCityBuildingEnum> BuildingType;
};
