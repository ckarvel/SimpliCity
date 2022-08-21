// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SimpliCityCharacter.generated.h"

UCLASS(Blueprintable)
class ASimpliCityCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASimpliCityCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void MoveByUnits(FVector translation);
	UFUNCTION()
	void RotateByUnits(FRotator rotator);

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "SimpliCityCharacter")
	class UCameraComponent* PlayerCameraComponent;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "SimpliCityCharacter")
	float moveSpeed;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "SimpliCityCharacter")
	float rotationSpeed;
};