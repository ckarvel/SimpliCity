// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SimpliCityZoneManager.generated.h"

UCLASS()
class SIMPLICITY_API ASimpliCityZoneManager : public AActor
{
	GENERATED_BODY()

public:
	ASimpliCityZoneManager();

protected:
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable,Category="SimpliCityZoneManager")
	void InitializeCellZones();

	UPROPERTY(EditAnywhere,NoClear,BlueprintReadOnly,Category = "SimpliCityZoneManager")
	TSubclassOf<class ASimpliCityZoneCell> ZoneCellClass;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = "SimpliCityZoneManager")
	TArray<class ASimpliCityZoneCell*> ZonedGridCells;
};
