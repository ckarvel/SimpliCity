// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintAssignable)
	FOnBuildCreation OnBuildCreation;
	UPROPERTY(BlueprintAssignable)
	FOnBuildRemoval OnBuildRemoval;

	UFUNCTION(BlueprintCallable,Category="BuildMode")
	void StartTrackingBuildPath(FVector location);
	UFUNCTION(BlueprintCallable,Category="BuildMode")
	void TrackBuildPath(FVector location);
	UFUNCTION(BlueprintCallable,Category="BuildMode")
	void FinishTrackingBuildPath();

	bool isTrackingActive;
	FVector startLocation;
	FVector lastLocation; // keep track of last tile origin
	UPROPERTY(BlueprintReadOnly,Category="BuildMode")
	TSet<FVector> oldPath;
};
