// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Builder/SimpliCityBuildObjectBase.h"
#include "SimpliCityRoadBase.generated.h"

UCLASS(Blueprintable)
class SIMPLICITY_API ASimpliCityRoadBase : public ASimpliCityBuildObjectBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASimpliCityRoadBase();

	UFUNCTION(BlueprintCallable,Category="Road Base")
	FORCEINLINE class UStaticMeshComponent* GetStaticMeshComponent() const { return StaticMeshComponent; }

	UFUNCTION(BlueprintCallable,Category="Road Base")
	void SetNewMaterial() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Road)
	class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Road)
	class UMaterialInstance* RoadMaterial;
};
