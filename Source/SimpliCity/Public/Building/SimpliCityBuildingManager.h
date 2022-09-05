// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Building/SimpliCityBuildingEnum.h"
#include "SimpliCityBuildingManager.generated.h"

UCLASS()
class SIMPLICITY_API ASimpliCityBuildingManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASimpliCityBuildingManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="SimpliCityBuildingManager")
	bool BuildModeEnabled;

public:
	UFUNCTION(BlueprintCallable,Category="SimpliCityBuildingManager")
	class ASimpliCityBuildingBase* GetAnyBuildingWithSupply(ESimpliCityBuildingEnum Type);
	UFUNCTION(BlueprintCallable,Category="SimpliCityBuildingManager")
	void AddBuildingToList(ESimpliCityBuildingEnum Type, ASimpliCityBuildingBase* Building);
	UFUNCTION(BlueprintCallable,Category="SimpliCityBuildingManager")
	void RemoveBuildingFromList(ESimpliCityBuildingEnum Type, ASimpliCityBuildingBase* Building);

	TMap<ESimpliCityBuildingEnum,TArray<ASimpliCityBuildingBase*>> BuildingListPerType;
};
