// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SimpliCityCharacter.generated.h"

UCLASS(Blueprintable)
class ASimpliCityCharacter : public ACharacter {
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
  void ZoomByUnits(float axis);
  UFUNCTION()
  void RotateByUnits(FRotator rotator);
  UFUNCTION()
  void SetRotationCenter(FVector Location);
  void OrbitAroundCenter(float angle);

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SimpliCityCharacter")
  class UCameraComponent* PlayerCameraComponent;
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SimpliCityCharacter")
  class USpringArmComponent* SpringArmComponent;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpliCityCharacter")
  float moveSpeed;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpliCityCharacter")
  float rotationSpeed;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpliCityCharacter")
  float zoomSpeed;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpliCityCharacter")
  float minZoom;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpliCityCharacter")
  float maxZoom;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpliCityCharacter")
  bool orbitMode;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpliCityCharacter")
  FVector rotationCenter;
};
