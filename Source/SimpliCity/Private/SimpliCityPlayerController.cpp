// Copyright Epic Games, Inc. All Rights Reserved.

#include "SimpliCityPlayerController.h"
#include "SimpliCityCharacter.h"
#include "Tools/SimpliCityUtils.h"

#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/World.h"

ASimpliCityPlayerController::ASimpliCityPlayerController()
  : CurrentInteractionMode(ESimpliCityInteractionMode::InteractionMode_None)
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

void ASimpliCityPlayerController::BeginPlay() {
  ThePlayer = Cast<ASimpliCityCharacter>(GetPawn());
  if (ThePlayer == nullptr)
    return;
  // playerstart has a rotation that only affects controller
  // here we set pawns rotation to controller's
  ThePlayer->SetActorRotation(GetRootComponent()->GetComponentRotation());

  ResetBindings();

  Super::BeginPlay();
}

void ASimpliCityPlayerController::ClearBindings() {
  OnArrowInput.Clear();
  OnMouseClick.Clear();
  OnMouseUp.Clear();
  OnMouseCancel.Clear();
  OnMouseHold.Clear();
  OnMiddleMouseClick.Clear();
  OnMiddleMouseHold.Clear();
}

void ASimpliCityPlayerController::ResetBindings() {
  ClearBindings();
  OnArrowInput.AddDynamic(ThePlayer,&ASimpliCityCharacter::MoveByUnits);
  OnMiddleMouseClick.AddDynamic(ThePlayer,&ASimpliCityCharacter::SetRotationCenter);
  OnMiddleMouseHold.AddDynamic(ThePlayer,&ASimpliCityCharacter::RotateByUnits);
}

void ASimpliCityPlayerController::PlayerTick(float DeltaTime) {
  Super::PlayerTick(DeltaTime);

  FHitResult TraceHitResult;
  GetHitResultUnderCursor(ECC_Visibility,false,TraceHitResult);
  bool blocked = TraceHitResult.bBlockingHit;
  FVector location = TraceHitResult.Location;
  TickHitLocation = location;

  HandleInputEvents(blocked,location);
}

//
//void ASimpliCityPlayerController::ClearAllBindings() {
//  ClearBindings();
//  OnMouseHover.RemoveAll();
//}

void ASimpliCityPlayerController::HandleInputEvents(bool blockingHit,FVector location) {
  // move camera
  OnArrowInput.Broadcast(FVector(GetInputAxisValue("MoveForward"),GetInputAxisValue("MoveRight"),0));

  if (blockingHit) {
     // we prob dont need this... highlighting stuff should be done w/ overlap events
     OnMouseHover.Broadcast(location); 
  }

  // right-click or esc for cancel
  if (WasInputKeyJustPressed(EKeys::RightMouseButton) || WasInputKeyJustPressed(EKeys::Escape)) {
    OnMouseCancel.Broadcast();
    return;
  }

  bool rotating = false;

  if (WasInputKeyJustReleased(EKeys::LeftMouseButton)
    || WasInputKeyJustReleased(EKeys::RightMouseButton)
    || WasInputKeyJustReleased(EKeys::MiddleMouseButton)) {
    OnMouseUp.Broadcast();
    SetInputMode(FInputModeGameAndUI());
  }
  else if (blockingHit && IsInputKeyDown(EKeys::LeftMouseButton)) {
    if (WasInputKeyJustPressed(EKeys::LeftMouseButton))
      OnMouseClick.Broadcast(location);
    else
      OnMouseHold.Broadcast(location);
  }
  else if (IsInputKeyDown(EKeys::MiddleMouseButton)) {
    if (WasInputKeyJustPressed(EKeys::MiddleMouseButton)) {
      OnMiddleMouseClick.Broadcast(location);
    }
    else {
      FRotator rotation = FRotator(GetInputAxisValue("RotationY"),GetInputAxisValue("RotationX"),0);
      OnMiddleMouseHold.Broadcast(rotation);
    }
    rotating = true;
    SetInputMode(FInputModeGameOnly()); // allows cursor to go past the screen edges
  }
  else if (rotating == false) { // don't zoom while rotating
    float zoomAxis = GetInputAxisValue("Zoom");
    ThePlayer->ZoomByUnits(zoomAxis);
  }
}

void ASimpliCityPlayerController::UpdateInteractionMode(ESimpliCityInteractionMode Mode,bool IsEnabled) {
  if (IsEnabled) {
    CurrentInteractionMode = Mode;
  } else {
    CurrentInteractionMode = ESimpliCityInteractionMode::InteractionMode_None;
    ResetBindings();
  }
  OnInteractionMode.Broadcast(Mode,IsEnabled);
}

void ASimpliCityPlayerController::SetupInputComponent() {
  // set up gameplay key bindings
  Super::SetupInputComponent();
  InputComponent->BindAxis("MoveForward");
  InputComponent->BindAxis("MoveRight");
  InputComponent->BindAxis("RotationX");
  InputComponent->BindAxis("RotationY");
  InputComponent->BindAxis("Zoom");
  //InputComponent->AddActionBinding(RoadModePressed);

  //// workaround for using lambda with BindAction:
  FInputActionBinding RoadModePressed("RoadMode",IE_Pressed);
  RoadModePressed.ActionDelegate.GetDelegateForManualSet().BindLambda([this]() {
    // if not in another build mode, enable
    if (CurrentInteractionMode == ESimpliCityInteractionMode::InteractionMode_None) {
      UpdateInteractionMode(ESimpliCityInteractionMode::InteractionMode_Road, true);
    }
    // if already in road mode, disable
    else if (CurrentInteractionMode == ESimpliCityInteractionMode::InteractionMode_Road) {
      UpdateInteractionMode(ESimpliCityInteractionMode::InteractionMode_Road, false);
    }
    // if in another build mode, disable that mode, then enable road
    else {
      UpdateInteractionMode(CurrentInteractionMode, false);
      UpdateInteractionMode(ESimpliCityInteractionMode::InteractionMode_Road, true);
    }
    /*TRACE_SCREENMSG_PRINTF("RoadMode");*/
  });
}
