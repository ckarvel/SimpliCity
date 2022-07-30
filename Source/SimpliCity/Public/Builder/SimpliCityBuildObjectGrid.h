// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SimpliCityBuildObjectGrid.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMPLICITY_API USimpliCityBuildObjectGrid : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USimpliCityBuildObjectGrid();
	UFUNCTION(BlueprintCallable,Category="Object Grid")
	bool DoesObjectExistHere(FVector location);
	void NotifySpawnedObject(class ASimpliCityBuildObjectBase* SpawnedObject);
	void NotifyDespawnedObject(class ASimpliCityBuildObjectBase* DespawnedObject);
	void NotifyDespawnedObjectAtLocation(FVector location);
	class ASimpliCityBuildObjectBase* GetObjectAtLocation(FVector location);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = BuildObjectGrid)
	TArray<class ASimpliCityBuildObjectBase*> BuildObjectRefGrid;
};
