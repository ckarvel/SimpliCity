// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SimpliCity.h"
#include "GameFramework/Actor.h"
#include "SimpliCityObjectType.h"
#include "SimpliCityObjectManager.generated.h"

class ASimpliCityObjectBase;

UCLASS(Blueprintable)
class SIMPLICITY_API ASimpliCityObjectManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ASimpliCityObjectManager();

	UFUNCTION(BlueprintCallable,Category="SimpliCityObjectManager")
	void InitializeArray(int NumElements);

	UFUNCTION(BlueprintCallable,Category="SimpliCityObjectManager")
	bool DoesObjectExistHere(FVector Location);

	UFUNCTION(BlueprintCallable,Category="SimpliCityObjectManager")
	void AddObjectToGrid(ASimpliCityObjectBase* AddedObject);

	UFUNCTION(BlueprintCallable,Category="SimpliCityObjectManager")
	void ReplaceObjectInGrid(ASimpliCityObjectBase* OldObject, ASimpliCityObjectBase* NewObject);

	UFUNCTION(BlueprintCallable,Category="SimpliCityObjectManager")
	void RemoveObjectFromGrid(ASimpliCityObjectBase* RemovedObject);

	UFUNCTION(BlueprintCallable,Category="SimpliCityObjectManager")
	void RemoveObjectAtLocation(FVector Location);

	UFUNCTION(BlueprintCallable,Category="SimpliCityObjectManager")
	void User_RemoveObjectFromGrid(ASimpliCityObjectBase* RemovedObject);

	UFUNCTION(BlueprintCallable,Category="SimpliCityObjectManager")
	ASimpliCityObjectBase* GetObjectAtLocation(FVector Location);

	UFUNCTION(BlueprintCallable,Category="SimpliCityObjectManager")
	TArray<ASimpliCityObjectBase*> GetNeighborsOfType(FVector Location,TEnumAsByte<ESimpliCityObjectType> ObjectType);

	UFUNCTION(BlueprintCallable,Category="SimpliCityObjectManager")
	TArray<ASimpliCityObjectBase*> GetAllNeighbors(FVector Location);

	UFUNCTION(BlueprintCallable,Category="SimpliCityObjectManager")
	bool IsInitialized() { return bInitialized; }

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = "SimpliCityObjectManager")
	TArray<ASimpliCityObjectBase*> ObjectGrid;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "SimpliCityObjectManager")
	bool bInitialized;
};
