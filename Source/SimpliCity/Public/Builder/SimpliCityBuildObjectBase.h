// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Builder/SimpliCityBuildObjectEnum.h"
#include "SimpliCityBuildObjectBase.generated.h"

UCLASS()
class SIMPLICITY_API ASimpliCityBuildObjectBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASimpliCityBuildObjectBase();
	UFUNCTION(BlueprintPure,Category="SimpliCityBuildObjectBase")
	FORCEINLINE TEnumAsByte<ESimpliCityBuildObjectEnum> Type() const { return BuildType; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void SetNewMaterial();
protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "SimpliCityBuildObjectBase")
	TEnumAsByte<ESimpliCityBuildObjectEnum> BuildType;
};
