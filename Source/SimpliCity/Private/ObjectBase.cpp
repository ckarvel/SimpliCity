// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectBase.h"

AObjectBase::AObjectBase()
{
	PrimaryActorTick.bCanEverTick = false;

}

//////////////////////////////////////////////////////////////////////////
void AObjectBase::BeginPlay()
{
	Super::BeginPlay();
}
