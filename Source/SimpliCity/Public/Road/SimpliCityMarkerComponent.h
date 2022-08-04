// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SimpliCityMarkerComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMPLICITY_API USimpliCityMarkerComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	USimpliCityMarkerComponent();

protected:
	virtual void BeginPlay() override;

public:	
  //UFUNCTION(BlueprintCallable,Category = "Marker")
  //void AddAdjacentMarker(USimpliCityMarkerComponent* marker);

  //UFUNCTION(BlueprintCallable,Category = "Marker")
  //TArray<FVector> GetAdjacentMarkerLocations();

  //UFUNCTION(BlueprintCallable,Category = "Marker")
  //TArray<USimpliCityMarkerComponent*> GetAdjacentMarkers() { return AdjacentMarkers; }

  //UPROPERTY(EditAnywhere,BlueprintReadWrite)
  //bool openForConnections;

  //UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
  //TArray<USimpliCityMarkerComponent*> AdjacentMarkers;

  //UFUNCTION(BlueprintCallable,Category = "Marker")
  //bool IsOpen() const { return openForConnections; }
};
