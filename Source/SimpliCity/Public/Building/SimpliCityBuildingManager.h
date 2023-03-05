// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Building/SimpliCityBuildingType.h"
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
	class ASimpliCityBuildingBase* GetAnyBuildingWithSupply(ESimpliCityBuildingType Type);
	UFUNCTION(BlueprintCallable,Category="SimpliCityBuildingManager")
	void AddBuildingToList(ESimpliCityBuildingType Type, ASimpliCityBuildingBase* Building);
	UFUNCTION(BlueprintCallable,Category="SimpliCityBuildingManager")
	void RemoveBuildingFromList(ESimpliCityBuildingType Type, ASimpliCityBuildingBase* Building);

	TMap<ESimpliCityBuildingType,TArray<ASimpliCityBuildingBase*>> BuildingListPerType;
};
