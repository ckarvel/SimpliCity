// Copyright Epic Games, Inc. All Rights Reserved.

#include "SimpliCityPlayerController.h"
#include "SimpliCityUtils.h"

#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/World.h"

ASimpliCityPlayerController::ASimpliCityPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

void ASimpliCityPlayerController::BeginPlay() {
  //ThePlayer = Cast<ASimpliCityCharacter>(GetPawn());
  //if (ThePlayer == nullptr)
  //  return;

  //OnArrowInput.BindDynamic(ThePlayer,&ASimpliCityCharacter::MoveByUnits);
  //OnMiddleMouseClick.BindDynamic(ThePlayer,&ASimpliCityCharacter::RotateByUnits);
  Super::BeginPlay();
}

void ASimpliCityPlayerController::PlayerTick(float DeltaTime) {
  Super::PlayerTick(DeltaTime);

  FHitResult TraceHitResult;
  GetHitResultUnderCursor(ECC_Visibility,false,TraceHitResult);
  bool blocked = TraceHitResult.bBlockingHit;
  FVector location = TraceHitResult.Location;

  HandleHoverEvent(blocked,location);
  HandleClickEvent(blocked,location);
  HandleArrowEvent();
}

//void ASimpliCityPlayerController::ClearBindings() {
//  OnMouseClick.RemoveAll();
//  OnMouseUp.RemoveAll();
//  OnMouseCancel.RemoveAll();
//  OnMouseHold.RemoveAll();
//}
//
//void ASimpliCityPlayerController::ClearAllBindings() {
//  ClearBindings();
//  OnMouseHover.RemoveAll();
//}

void ASimpliCityPlayerController::HandleHoverEvent(bool blockingHit,FVector location) {
  if (blockingHit == false)
    return;
  //if (OnMouseHover.IsBound() == false)
  //  OnMouseHover.BindUObject(gridManager,&AGridManager::SetSelectedTile);
  OnMouseHover.Broadcast(location);
}

void ASimpliCityPlayerController::HandleClickEvent(bool blockingHit,FVector location) {
  // right-click or esc for cancel
  if (WasInputKeyJustPressed(EKeys::RightMouseButton) || WasInputKeyJustPressed(EKeys::Escape))
    OnMouseCancel.Broadcast();
  // left-click for placement
  else if (blockingHit && IsInputKeyDown(EKeys::LeftMouseButton)) {
    if (WasInputKeyJustPressed(EKeys::LeftMouseButton))
      OnMouseClick.Broadcast(location);
    else
      OnMouseHold.Broadcast(location);
  } else if (WasInputKeyJustReleased(EKeys::LeftMouseButton) || WasInputKeyJustReleased(EKeys::RightMouseButton))
    OnMouseUp.Broadcast();
}

void ASimpliCityPlayerController::HandleArrowEvent() {
  // move camera
  OnArrowInput.Broadcast(FVector(GetInputAxisValue("MoveForward"),GetInputAxisValue("MoveRight"),0));

  // rotate camera
  if (IsInputKeyDown(EKeys::MiddleMouseButton)) {
    // Pitch, Yaw, Roll
    // Pitch = Y 
    // Yaw = X
    float axisy = GetInputAxisValue("RotationY");
    float axisx = GetInputAxisValue("RotationX");
    OnMiddleMouseClick.Broadcast(FRotator(axisy,axisx,0));
    //if (axisy != 0.00000)
    //  SIMPLI_LOG(TEXT("Pitch: %.2f"),axisy);
    //if (axisx != 0.000000)
    //  SIMPLI_LOG(TEXT("Yaw: %.2f"),axisx);
  }
}

void ASimpliCityPlayerController::SetupInputComponent() {
  // set up gameplay key bindings
  Super::SetupInputComponent();
  InputComponent->BindAxis("MoveForward");
  InputComponent->BindAxis("MoveRight");
  InputComponent->BindAxis("RotationX");
  InputComponent->BindAxis("RotationY");
}