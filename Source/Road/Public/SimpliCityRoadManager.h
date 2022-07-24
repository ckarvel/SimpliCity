// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SimpliCityRoadManager.generated.h"

UCLASS(Blueprintable)
class ROAD_API ASimpliCityRoadManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASimpliCityRoadManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TArray<class ASimpliCityRoadBase*> PermanentRoadList;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TArray<class ASimpliCityRoadBase*> TemporaryRoadList;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
