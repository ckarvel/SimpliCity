// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Builder/SimpliCityBuildObjectBase.h"
#include "SimpliCityRoadBase.generated.h"

UCLASS(Blueprintable)
class SIMPLICITY_API ASimpliCityRoadBase : public ASimpliCityBuildObjectBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASimpliCityRoadBase();

	UFUNCTION(BlueprintCallable,Category="SimpliCityRoadBase")
	FORCEINLINE class UStaticMeshComponent* GetStaticMeshComponent() const { return StaticMeshComponent; }

	UFUNCTION(BlueprintCallable,Category="SimpliCityRoadBase")
	void SetNewMaterial() override;

	UFUNCTION(BlueprintCallable,Category="SimpliCityRoadBase")
	FVector GetVehMarkerNormDirectionVector(class UMarkerComponent* VMarker);

	UFUNCTION(BlueprintCallable,Category="SimpliCityRoadBase")
	TArray<UMarkerComponent*> GetClosestMarkerPair(ASimpliCityRoadBase* NeighborRoad, bool IsPedestrian);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="SimpliCityRoadBase")
	class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="SimpliCityRoadBase")
	class UMaterialInstance* RoadMaterial;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="SimpliCityRoadBase")
	TArray<class UMarkerComponent*> PedestrianMarkers;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="SimpliCityRoadBase")
	TArray<class UMarkerComponent*> VehicleMarkers;
};
