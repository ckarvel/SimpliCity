// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SimpliCity.h"
#include "GameFramework/Actor.h"
#include "SimpliCityZoneType.h"
#include "SimpliCityZoneBase.h"
#include "SimpliCityZoneManager.generated.h"

UCLASS()
class SIMPLICITY_API ASimpliCityZoneManager : public AActor
{
	GENERATED_BODY()

public:
	ASimpliCityZoneManager();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="SimpliCityZoneManager")
	bool BuildModeEnabled;

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
	TEnumAsByte<ESimpliCityZoneType> GetZoneTypeAtLocation(FVector Location);

	UFUNCTION(BlueprintCallable,Category="SimpliCityZoneManager")
	TArray<class ASimpliCityZoneCell*> GetUnfilledZonedCells();

	UPROPERTY(EditAnywhere,NoClear,BlueprintReadOnly,Category = "SimpliCityZoneManager")
	TSubclassOf<ASimpliCityZoneCell> ZoneCellClass;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = "SimpliCityZoneManager")
	TArray<ASimpliCityZoneCell*> EmptyZones;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = "SimpliCityZoneManager")
	TArray<ASimpliCityZoneCell*> ZonedGridCells;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = "SimpliCityZoneManager")
	TMap<ASimpliCityZoneCell*, TEnumAsByte<ESimpliCityZoneType>> TemporaryCellStates;

	TMap<ESimpliCityZoneType, TArray<ASimpliCityZoneBase*>> ZoneBasesPerType;
};
