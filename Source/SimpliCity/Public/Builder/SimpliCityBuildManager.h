// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Builder/SimpliCityBuildObjectEnum.h"
#include "SimpliCityBuildManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBuildCreation,const TArray<FVector>&,AddedLocations);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBuildRemoval,const TArray<FVector>&,RemovedLocations);

UCLASS()
class SIMPLICITY_API ASimpliCityBuildManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASimpliCityBuildManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintAssignable)
	FOnBuildCreation OnBuildCreation;
	UPROPERTY(BlueprintAssignable)
	FOnBuildRemoval OnBuildRemoval;

	UFUNCTION(BlueprintCallable,Category="Build Mode")
	bool DoesObjectExistHere(FVector Location);

	UFUNCTION(BlueprintCallable,Category="Build Mode")
	void NotifySpawnedObject(class ASimpliCityBuildObjectBase* SpawnedObject);

	UFUNCTION(BlueprintCallable,Category="Build Mode")
	void NotifyDespawnedObject(ASimpliCityBuildObjectBase* DespawnedObject);
	UFUNCTION(BlueprintCallable,Category="Build Mode")
	void NotifyDespawnedObjectAtLocation(FVector location);
	void NotifyReplaceObject(ASimpliCityBuildObjectBase* OldObj,ASimpliCityBuildObjectBase* NewObj);

	UFUNCTION(BlueprintCallable,Category="Build Mode")
	class ASimpliCityBuildObjectBase* GetObjectAtLocation(FVector location);
	UFUNCTION(BlueprintCallable,Category="Build Mode")
	TArray<ASimpliCityBuildObjectBase*> GetNeighborsOfType(FVector location,TEnumAsByte<ESimpliCityBuildObjectEnum> buildType);
	TArray<ASimpliCityBuildObjectBase*> GetAllNeighbors_Unsafe(FVector location);

protected:
	UFUNCTION(BlueprintCallable,Category="Build Mode")
	void StartTrackingBuildPath(FVector location);
	UFUNCTION(BlueprintCallable,Category="Build Mode")
	void TrackBuildPath(FVector location);
	UFUNCTION(BlueprintCallable,Category="Build Mode")
	void FinishTrackingBuildPath();
	void AddTemporaryToPermanentList();

	bool isTrackingActive;
	FVector startLocation;
	FVector lastLocation; // keep track of last tile origin
	UPROPERTY(BlueprintReadOnly,Category="Build Mode")
	TSet<FVector> oldPath;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TArray<class ASimpliCityBuildObjectBase*> TemporaryBuildObjects;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TArray<FVector> TemporaryBuildLocations;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	class USimpliCityBuildObjectGrid* ObjectGridComponent;
};
