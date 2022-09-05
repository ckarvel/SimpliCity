// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpliCityObjectBase.h"

// Sets default values
ASimpliCityObjectBase::ASimpliCityObjectBase()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ASimpliCityObjectBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called when the game starts or when spawned
void ASimpliCityObjectBase::SetNewMaterial() {
	// implement
}