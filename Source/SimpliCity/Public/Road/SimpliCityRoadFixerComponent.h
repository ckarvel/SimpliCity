// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SimpliCityRoadFixerComponent.generated.h"


UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMPLICITY_API USimpliCityRoadFixerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USimpliCityRoadFixerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere,NoClear,BlueprintReadOnly,Category = "SimpliCityRoadFixer")
	TSubclassOf<class ASimpliCityRoadBase> Road4WayClass;
	UPROPERTY(EditAnywhere,NoClear,BlueprintReadOnly,Category = "SimpliCityRoadFixer")
	TSubclassOf<ASimpliCityRoadBase> Road3WayClass;
	UPROPERTY(EditAnywhere,NoClear,BlueprintReadOnly,Category = "SimpliCityRoadFixer")
	TSubclassOf<ASimpliCityRoadBase> RoadStraightClass;
	UPROPERTY(EditAnywhere,NoClear,BlueprintReadOnly,Category = "SimpliCityRoadFixer")
	TSubclassOf<ASimpliCityRoadBase> RoadCornerClass;
	UPROPERTY(EditAnywhere,NoClear,BlueprintReadOnly,Category = "SimpliCityRoadFixer")
	TSubclassOf<ASimpliCityRoadBase> RoadDeadEndClass;
	UFUNCTION(BlueprintCallable,Category = "SimpliCityRoadFixer")
	bool IsRoad(class ASimpliCityBuildObjectBase* obj);
	UFUNCTION(BlueprintCallable,Category = "SimpliCityRoadFixer")
	void GetRoadTypeAndRotationAtLocation(FVector location, TSubclassOf<class ASimpliCityRoadBase>& OutRoadClass, FRotator& OutRotation);

private:
	void Set4WayRoadInfo(FVector location,TArray<ASimpliCityBuildObjectBase*> neighbors,TSubclassOf<ASimpliCityRoadBase>& OutRoadClass,FRotator& OutRotation);
	void Set3WayRoadInfo(FVector location,TArray<ASimpliCityBuildObjectBase*> neighbors,TSubclassOf<ASimpliCityRoadBase>& OutRoadClass,FRotator& OutRotation);
	bool SetStraightRoadInfo(FVector location,TArray<ASimpliCityBuildObjectBase*> neighbors,TSubclassOf<ASimpliCityRoadBase>& OutRoadClass,FRotator& OutRotation);
	void SetCornerWayRoadInfo(FVector location,TArray<ASimpliCityBuildObjectBase*> neighbors,TSubclassOf<ASimpliCityRoadBase>& OutRoadClass,FRotator& OutRotation);
	void SetDeadEndRoadInfo(FVector location,TArray<ASimpliCityBuildObjectBase*> neighbors,TSubclassOf<ASimpliCityRoadBase>& OutRoadClass,FRotator& OutRotation);
};
