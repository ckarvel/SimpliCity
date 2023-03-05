// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SimpliCityObjectBase.h"
#include "SimpliCityZoneType.h"
#include "SimpliCityZoneBase.generated.h"

UCLASS()
class SIMPLICITY_API ASimpliCityZoneBase : public ASimpliCityObjectBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASimpliCityZoneBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintCallable,Category="SimpliCityZoneBase")
	FORCEINLINE class UStaticMeshComponent* GetStaticMeshComponent() const { return StaticMeshComponent; }

	UFUNCTION(BlueprintCallable,Category="SimpliCityZoneBase")
	class ASimpliCityRoadBase* GetRoadConnection() const { return RoadConnection; }

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="SimpliCityZoneBase")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="SimpliCityZoneBase")
	ASimpliCityRoadBase* RoadConnection;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="SimpliCityZoneBase",Meta = (ExposeOnSpawn=true))
	TEnumAsByte<ESimpliCityZoneType> ZoneType;
};
