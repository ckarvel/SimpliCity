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

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<class ASimpliCityRoadBase*> RoadList;

public:
	UFUNCTION(BlueprintCallable,Category = "Road")
	virtual TArray<FVector> GetNeighbors(FVector Location) const override;

	UFUNCTION(BlueprintCallable,Category = "Road")
	virtual float GetCost(FVector Location) const override { return 1.0; } // todo

	void FixRoad(class ASimpliCityBuildObjectBase* Road);

	UFUNCTION(BlueprintCallable,Category="Road")
	void FixNeighborsOfRoad(ASimpliCityBuildObjectBase* Road);

	void SwapRoads(ASimpliCityBuildObjectBase* OldRoad,ASimpliCityBuildObjectBase* NewRoad);

	UFUNCTION(BlueprintCallable,Category="Road")
	void FixRoadAndNeighbors(ASimpliCityBuildObjectBase* Road);

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	class USimpliCityRoadFixerComponent* RoadFixerComponent;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class AMarkerManager* AgentMarkerGraph;
};
