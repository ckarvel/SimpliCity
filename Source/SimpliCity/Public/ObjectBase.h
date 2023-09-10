// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SimpliCityObjectType.h"
#include "SimpliCityResourceTypes.h"
#include "ObjectBase.generated.h"

UCLASS(Blueprintable)
class SIMPLICITY_API AObjectBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObjectBase();

	UFUNCTION(BlueprintPure, Category = "ObjectBase")
  FORCEINLINE ESimpliCityObjectType GetObjectType() const {
    return ObjectType;
  }

	FORCEINLINE ESimpliCityResourceType GetResourceType() const {
    return ResourceType;
  }

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ObjectBase")
  void OnPlaced();

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ObjectBase")
  void SetPlacementState(bool State);

  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ObjectBase")
  bool IsValidPlacement();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (ExposeOnSpawn = true), Category = "ObjectBase")
  ESimpliCityObjectType ObjectType;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "ObjectBase")
  ESimpliCityResourceType ResourceType;
};
