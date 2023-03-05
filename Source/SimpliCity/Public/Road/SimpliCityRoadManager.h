// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PathFinderInterface.h"
#include "SimpliCityRoadManager.generated.h"

class AMarkerManager;
class ASimpliCityObjectBase;
class ASimpliCityRoadBase;
class USimpliCityRoadFixerComponent;

UCLASS(Blueprintable)
class SIMPLICITY_API ASimpliCityRoadManager : public AActor, public IPathFinderInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASimpliCityRoadManager();

protected:
	//////////////////////////////////////////////////////////////////////////
	// build/destroy functions
	//////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintCallable,Category="SimpliCityBuildManager")
	void UpdatePath(FVector Location);
	//////////////////////////////////////////////////////////////////////////
	// build functions
	UFUNCTION(BlueprintCallable,Category="SimpliCityBuildManager")
	void StartPlacingRoad(FVector Location);
	UFUNCTION(BlueprintCallable,Category="SimpliCityBuildManager")
	void FinishBuildingPath();
	UFUNCTION(BlueprintCallable,Category="SimpliCityBuildManager")
	void CancelBuildingPath();
	UFUNCTION(BlueprintCallable,Category="SimpliCityBuildManager")
	bool PlacePermanentRoad(const FVector Location,const FRotator Rotation);
	UFUNCTION(BlueprintCallable,Category="SimpliCityBuildManager")
	void CreateTemporaryRoadsAtLocations(const TArray<FVector>& Locations);
	void DestroyTemporaryRoadsAtLocations(const TArray<FVector> Locations);
	void DestroyAllTemporaryRoads();
	UFUNCTION(BlueprintCallable,Category="SimpliCityBuildManager")
	void ConvertAllTemporaryToPermanent();
	//////////////////////////////////////////////////////////////////////////
	// destroy functions
	UFUNCTION(BlueprintCallable,Category="SimpliCityBuildManager")
	void DestroyPermanentRoad(ASimpliCityObjectBase* Road);

public:
	void FixRoad(ASimpliCityObjectBase* Road);
	void SwapRoads(ASimpliCityObjectBase* OldRoad,ASimpliCityObjectBase* NewRoad);

	UFUNCTION(BlueprintCallable,Category = "SimpliCityRoadManager")
	virtual TArray<FVector> GetNeighbors(FVector Location) const override;

	UFUNCTION(BlueprintCallable,Category = "SimpliCityRoadManager")
	virtual float GetCost(FVector Location) const override { return 1.0; } // todo

	UFUNCTION(BlueprintCallable,Category="SimpliCityRoadManager")
	void FixNeighborsAtLocation(FVector Location);

	UFUNCTION(BlueprintCallable,Category="SimpliCityRoadManager")
	void FixRoadAndNeighbors(ASimpliCityObjectBase* Road);

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="SimpliCityRoadManager")
	USimpliCityRoadFixerComponent* RoadFixerComponent;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="SimpliCityRoadManager")
	AMarkerManager* AgentMarkerGraph;

protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="SimpliCityRoadManager")
	bool IsCurrentlyBuilding;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="SimpliCityRoadManager")
	TMap<FVector,ASimpliCityRoadBase*> TemporaryRoadLocMap;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="SimpliCityRoadManager")
	TArray<ASimpliCityRoadBase*> PermanentRoadList;

private:
	FVector startLocation;
	FVector lastLocation; // keep track of last tile origin
	TSet<FVector> oldPath;
};
