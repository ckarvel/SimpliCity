// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PathFinderInterface.h"
#include "SimpliCityRoadManager.generated.h"

UCLASS(Blueprintable)
class SIMPLICITY_API ASimpliCityRoadManager : public AActor, public IPathFinderInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASimpliCityRoadManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="SimpliCityRoadManager")
	TArray<class ASimpliCityRoadBase*> RoadList;

public:
	void FixRoad(class ASimpliCityBuildObjectBase* Road);
	void SwapRoads(ASimpliCityBuildObjectBase* OldRoad,ASimpliCityBuildObjectBase* NewRoad);

	UFUNCTION(BlueprintCallable,Category = "SimpliCityRoadManager")
	virtual TArray<FVector> GetNeighbors(FVector Location) const override;

	UFUNCTION(BlueprintCallable,Category = "SimpliCityRoadManager")
	virtual float GetCost(FVector Location) const override { return 1.0; } // todo

	UFUNCTION(BlueprintCallable,Category="SimpliCityRoadManager")
	void FixNeighborsOfRoad(ASimpliCityBuildObjectBase* Road);

	UFUNCTION(BlueprintCallable,Category="SimpliCityRoadManager")
	void FixRoadAndNeighbors(ASimpliCityBuildObjectBase* Road);

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="SimpliCityRoadManager")
	class USimpliCityRoadFixerComponent* RoadFixerComponent;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="SimpliCityRoadManager")
	class AMarkerManager* AgentMarkerGraph;
};
