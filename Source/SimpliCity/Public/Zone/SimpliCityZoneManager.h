// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SimpliCityZoneTypeEnum.h"
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

	UFUNCTION(BlueprintCallable,Category="SimpliCityZoneManager")
	bool IsCellSelected(class ASimpliCityZoneCell* Cell);

	UFUNCTION(BlueprintCallable,Category="SimpliCityZoneManager")
	bool BackupCellDataIfNotExists(ASimpliCityZoneCell* Cell);

	UFUNCTION(BlueprintCallable,Category="SimpliCityZoneManager")
	void RestoreCellData(ASimpliCityZoneCell* Cell);

	UFUNCTION(BlueprintCallable,Category="SimpliCityZoneManager")
	void RestoreAllCellData();

	UFUNCTION(BlueprintCallable,Category="SimpliCityZoneManager")
	void ClearBackupData();

	UFUNCTION(BlueprintCallable,Category="SimpliCityZoneManager")
	TArray<class ASimpliCityZoneCell*> GetUnfilledZonedCells();

	UPROPERTY(EditAnywhere,NoClear,BlueprintReadOnly,Category = "SimpliCityZoneManager")
	TSubclassOf<ASimpliCityZoneCell> ZoneCellClass;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = "SimpliCityZoneManager")
	TArray<ASimpliCityZoneCell*> ZonesWithBuildings;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = "SimpliCityZoneManager")
	TArray<ASimpliCityZoneCell*> ZonedGridCells;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = "SimpliCityZoneManager")
	TMap<ASimpliCityZoneCell*, TEnumAsByte<ESimpliCityZoneTypeEnum>> TemporaryCellStates;
};
