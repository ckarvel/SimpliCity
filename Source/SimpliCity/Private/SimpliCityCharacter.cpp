// Copyright Epic Games, Inc. All Rights Reserved.

#include "SimpliCityCharacter.h"
#include "SimpliCityUtils.h"

#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"

ASimpliCityCharacter::ASimpliCityCharacter()
  : moveSpeed(10)
  , rotationSpeed(5) {
  PrimaryActorTick.bCanEverTick = false;

  PlayerCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
  SetRootComponent(PlayerCameraComponent);
}

void ASimpliCityCharacter::BeginPlay() {
  Super::BeginPlay();
}

void ASimpliCityCharacter::MoveByUnits(FVector translation) {
  FVector location = GetActorLocation();
  int moveForward = translation.X;
  int moveRight = translation.Y;

  if (moveForward)
    location += moveForward * GetActorForwardVector() * moveSpeed;
  if (moveRight)
    location += moveRight * GetActorRightVector() * moveSpeed;

  SetActorLocation(location);
}

void ASimpliCityCharacter::RotateByUnits(FRotator rotator) {
  if (rotator.IsNearlyZero())
    return;

  //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("%f"), rotator.Pitch));

  // IMPORTANT: Pitch affects Roll
  //   Pitch affects Roll when Pitch is changed by 90 degrees
  //   To prevent Roll changing: Clamp to -89 and 89.
  //SIMPLI_LOG(TEXT("Before Speed: Y: %.2f"),rotator.Yaw);
  rotator *= rotationSpeed;
  //SIMPLI_LOG(TEXT("After Speed: Y: %.2f"),rotator.Yaw);
  FRotator result = GetActorRotation() + rotator;
  result.Roll = 0.0;
  result.Yaw = FMath::Clamp(result.Yaw,-179.0f,179.0f);
  result.Pitch = FMath::Clamp(result.Pitch,-89.0f,89.0f);
  SetActorRotation(result);
  //SIMPLI_LOG(TEXT("Final: Y: %.2f"),result.Yaw);
}