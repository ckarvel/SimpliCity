// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Builder/SimpliCityBuildObjectEnum.h"
#include "SimpliCityBuildManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBuildCreation,const TArray<FVector>&,AddedLocations);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBuildRemoval,const TArray<FVector>&,RemovedLocations);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelection,const TArray<AActor*>&,SelectedActors);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBuildFinish);

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
	UPROPERTY(BlueprintAssignable,Category="SimpliCityBuildManager")
	FOnBuildCreation OnBuildCreation;
	UPROPERTY(BlueprintAssignable,Category="SimpliCityBuildManager")
	FOnBuildRemoval OnBuildRemoval;
	UPROPERTY(BlueprintAssignable,Category="SimpliCityBuildManager")
	FOnBuildFinish OnBuildFinish;
	UPROPERTY(BlueprintAssignable,BlueprintCallable,Category="SimpliCityBuildManager")
	FOnSelection OnActorSelection;

	UFUNCTION(BlueprintCallable,Category="SimpliCityBuildManager")
	bool DoesObjectExistHere(FVector Location);

	UFUNCTION(BlueprintCallable,Category="SimpliCityBuildManager")
	void NotifySpawnedObject(class ASimpliCityBuildObjectBase* SpawnedObject);

	UFUNCTION(BlueprintCallable,Category="SimpliCityBuildManager")
	void NotifyDespawnedObject(ASimpliCityBuildObjectBase* DespawnedObject);
	UFUNCTION(BlueprintCallable,Category="SimpliCityBuildManager")
	void NotifyDespawnedObjectAtLocation(FVector location);
	UFUNCTION(BlueprintCallable,Category="SimpliCityBuildManager")
	void NotifyDespawnAllTemporary();
	void NotifyReplaceObject(ASimpliCityBuildObjectBase* OldObj,ASimpliCityBuildObjectBase* NewObj);

	UFUNCTION(BlueprintCallable,Category="SimpliCityBuildManager")
	class ASimpliCityBuildObjectBase* GetObjectAtLocation(FVector location);
	UFUNCTION(BlueprintCallable,Category="SimpliCityBuildManager")
	TArray<ASimpliCityBuildObjectBase*> GetNeighborsOfType(FVector location,TEnumAsByte<ESimpliCityBuildObjectEnum> buildType);
	TArray<ASimpliCityBuildObjectBase*> GetAllNeighbors_Unsafe(FVector location);
	// to be called for zone manager's buildings
	UFUNCTION(BlueprintCallable,Category="SimpliCityBuildManager")
	void NotifyPermanentObject(ASimpliCityBuildObjectBase* PermObject);

protected:
	UFUNCTION(BlueprintCallable,Category="SimpliCityBuildManager")
	void StartTrackingBuildPath(FVector location);
	UFUNCTION(BlueprintCallable,Category="SimpliCityBuildManager")
	void TrackBuildPath(FVector location);
	UFUNCTION(BlueprintCallable,Category="SimpliCityBuildManager")
	void FinishTrackingBuildPath();
	UFUNCTION(BlueprintCallable,Category="SimpliCityBuildManager")
	void CancelTrackingBuildPath();
	void RemoveAllTemporaryObjects();

	bool isTrackingActive;
	FVector startLocation;
	FVector lastLocation; // keep track of last tile origin
	UPROPERTY(BlueprintReadOnly,Category="SimpliCityBuildManager")
	TSet<FVector> oldPath;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TArray<class ASimpliCityBuildObjectBase*> TemporaryBuildObjects;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TArray<FVector> TemporaryBuildLocations;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	class USimpliCityBuildObjectGrid* ObjectGridComponent;
};
