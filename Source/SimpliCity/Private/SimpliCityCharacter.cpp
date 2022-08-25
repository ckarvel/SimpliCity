// Copyright Epic Games, Inc. All Rights Reserved.

#include "SimpliCityCharacter.h"
#include "Tools/SimpliCityUtils.h"
#include "Tools/SimpliCityFunctionLibrary.h"

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
  : moveSpeed(35)
  , rotationSpeed(10)
  , zoomSpeed(50)
  , minZoom(1000)
  , maxZoom(30000) {
  PrimaryActorTick.bCanEverTick = false;
  bUseControllerRotationYaw = false;
  PlayerCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
  SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
  FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative,true);
  PlayerCameraComponent->AttachToComponent(SpringArmComponent,AttachmentRules);
  SetRootComponent(SpringArmComponent);
}

void ASimpliCityCharacter::BeginPlay() {
  Super::BeginPlay();
  SpringArmComponent->TargetArmLength = maxZoom;
}

void ASimpliCityCharacter::MoveByUnits(FVector translation) {
  if (translation.IsNearlyZero())
    return;
  FVector location = GetActorLocation();
  int moveForward = translation.X * moveSpeed;
  int moveRight = translation.Y * moveSpeed;

  FVector forwardVector = GetActorForwardVector();
  forwardVector.Normalize();
  FVector rightVector = GetActorRightVector();
  rightVector.Normalize();

  location += moveForward * forwardVector;
  location += moveRight * rightVector;
  location.Z = GetActorLocation().Z;

  if (location.Equals(GetActorLocation())) {
    return;
  }

  FVector new_location = USimpliCityFunctionLibrary::VInterpTo(GetActorLocation(), location, GetWorld()->GetDeltaSeconds(), moveSpeed);
  SetActorLocation(new_location);
}

// basically "start rotating"
void ASimpliCityCharacter::SetRotationCenter(FVector Location) {
  rotationCenter = Location;
  rotationCenter.Z = 1.0;
}

// orbit around the user's pov
void ASimpliCityCharacter::OrbitAroundCenter(float angle) {
  FVector location = GetActorLocation();
  FVector actorToFloorDir = location - rotationCenter;
  float actorToFloorLen = actorToFloorDir.Length();
  actorToFloorDir.Normalize();
  actorToFloorDir.RotateAngleAxis(angle, FVector3d(0,0,1));
}

void ASimpliCityCharacter::RotateByUnits(FRotator rotator) {
  if (rotator.IsNearlyZero())
    return;

  //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("%f"), rotator.Pitch));

  // IMPORTANT: Pitch affects Roll
  //   Pitch affects Roll when Pitch is changed by 90 degrees
  //   To prevent Roll changing: Clamp to -89 and 89.
  //SIMPLI_LOG(TEXT("Before Speed: Y: %.2f"),rotator.Yaw);
  //rotator *= rotationSpeed;
  //SIMPLI_LOG(TEXT("After Speed: Y: %.2f"),rotator.Yaw);
  FRotator current = GetActorRotation();
  float newPitch = FMath::Clamp(current.Pitch + rotator.Pitch * rotationSpeed,-85.0f, -50.0f);
  float newYaw = FMath::Clamp(current.Yaw + rotator.Yaw * rotationSpeed,-359.0f,359.0f);
  FRotator result = FRotator(newPitch,newYaw, 0.0);

  if (result.Equals(rotator)) {
    return;
  }

  FRotator new_rotation = USimpliCityFunctionLibrary::RInterpTo(GetActorRotation(),result,GetWorld()->GetDeltaSeconds(), rotationSpeed);
  SetActorRotation(new_rotation);

  if (orbitMode) {
    OrbitAroundCenter(rotator.Yaw);
  }
}

void ASimpliCityCharacter::ZoomByUnits(float axis) {
  float zoomDelta = axis * zoomSpeed;
  float zoomValue = SpringArmComponent->TargetArmLength + zoomDelta;
  float targetZoom = FMath::Clamp(zoomValue,minZoom,maxZoom);

  if (USimpliCityFunctionLibrary::IsNearlyEqual(targetZoom, SpringArmComponent->TargetArmLength)) {
    return;
  }

  float finalZoom = USimpliCityFunctionLibrary::FInterpTo(SpringArmComponent->TargetArmLength,targetZoom,GetWorld()->GetDeltaSeconds(), zoomSpeed);
  SpringArmComponent->TargetArmLength = finalZoom;
}