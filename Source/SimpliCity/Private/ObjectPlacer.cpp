// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPlacer.h"

#include "Kismet/GameplayStatics.h"

#include "GridManager.h"
#include "SimpliCityFunctionLibrary.h"
#include "ObjectManager.h"
#include "SimpliCityObjectSelector.h"
#include "ObjectBase.h"
#include "SimpliCityGameState.h"

using SCFL = USimpliCityFunctionLibrary;

//////////////////////////////////////////////////////////////////////////
AObjectPlacer::AObjectPlacer()
  : Enabled(false)
  , IsPlacing(false)
  , ObjectType(ESimpliCityObjectType::None)
  , StartLocation(FVector(-1, -1, -1))
  , LastLocation(FVector(-1, -1, -1))
{
  PrimaryActorTick.bCanEverTick = true;
  // PrimaryActorTick.SetTickFunctionEnable(true);
  PrimaryActorTick.bStartWithTickEnabled = false;
}

//////////////////////////////////////////////////////////////////////////
void AObjectPlacer::BeginPlay()
{
  Super::BeginPlay();
  GameState = Cast<ASimpliCityGameState>(UGameplayStatics::GetGameState(this));
  GridManager = SCFL::GetGridManager(this);
  ObjectManager = SCFL::GetObjectManager(this);
  ObjectSelector = SCFL::GetSelector(this);
}

//////////////////////////////////////////////////////////////////////////
void AObjectPlacer::Enable(bool shouldEnable, ESimpliCityObjectType objectType, ESimpliCityResourceType resourceType) {
  if (ObjectType != objectType || ResourceType != resourceType) {
    if (shouldEnable) {
      // start placement mode for the first time
      // OR
      // user requests to place a different object/resource type, so restart
      StartPlacement(objectType, resourceType);
    } else {
      // asked to disable mode but object/resource type doesn't match, do nothing
      return;
    }
  } else if (shouldEnable == false) {
    // end/cancel placement mode
    Cleanup();
  }
  Enabled = shouldEnable;
}

//////////////////////////////////////////////////////////////////////////
void AObjectPlacer::StartPlacement(ESimpliCityObjectType objectType, ESimpliCityResourceType resourceType) {
  ObjectType = objectType;
  ResourceType = resourceType;

  // destroying any existing
  Cleanup();

  // spawn our first object
  AObjectBase* Actor = SpawnObject(ObjectType, ResourceType, FVector(0, 0, -1000));
  FVector ActorScale = FVector(0.05, 0.05, 0.05);
  if (ObjectType == ESimpliCityObjectType::Road) {
    ActorScale = FVector(0.5, 0.5, 0.05);
  }
  Actor->SetActorScale3D(ActorScale);
  TemporaryObjectLocationMap.Add(FVector(), Actor);

  ObjectSelector->VisualizeSelection = false;
  PrimaryActorTick.SetTickFunctionEnable(true);
}

//////////////////////////////////////////////////////////////////////////
//void AObjectPlacer::ConvertTemporariesToPermanent() {
//  for (auto ObjectPair : TemporaryObjectLocationMap) {
//    auto ObjectLoc = ObjectPair.Key;
//    auto Object = ObjectPair.Value;
//    if (Object == nullptr)
//      continue;
//    // move elsewhere
//    AObjectBase* OldObject = ObjectManager->GetObject(Object->GetActorLocation());
//    if (OldObject != nullptr) {
//      ObjectManager->RemoveObject(OldObject);
//      OldObject->Destroy();
//      OldObject = nullptr;
//    }
//    ObjectManager->AddObject(Object);
//    Object->OnPlaced();
//  }
//  TemporaryObjectLocationMap.Empty();
//}

//////////////////////////////////////////////////////////////////////////
void AObjectPlacer::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
  APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
  if (PlayerController) {
    FHitResult HitResult;
    PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, HitResult);
    if (HitResult.bBlockingHit) {
      FVector Location = GridManager->LocationToCenter(HitResult.Location);
      if (Location != LastLocation) {
        UpdatePlacement(Location);
      }
      LastLocation = Location;
    }
  }
}

//////////////////////////////////////////////////////////////////////////
bool AObjectPlacer::FinishPlacement(bool UserCancelled) {
  if (IsPlacing && UserCancelled) {
    // just start over (handles cleanup of existing assets)
    StartPlacement(ObjectType, ResourceType);
    return true;
  }
  else if (UserCancelled) {
    // just start over (handles cleanup of existing assets)
    Cleanup();
    return true;
  }
  // don't allow finishing if user can't afford it
  if (IsPlacementValid()) {
    // otherwise, place the new objects and start over
    ConvertTemporariesToPermanent();
    StartPlacement(ObjectType, ResourceType);
    return true;
  }
  IsPlacing = false;
  OnPlacementError.Broadcast();
  return false; // error 
}

//////////////////////////////////////////////////////////////////////////
// All Temporary Objects should have been permanent by now so we're ok to
// delete all temporaries if any still exist
void AObjectPlacer::Cleanup() {
  for (auto ObjectPair : TemporaryObjectLocationMap) {
    auto ObjectLoc = ObjectPair.Key;
    auto Object = ObjectPair.Value;
    if (Object != nullptr) {
      Object->Destroy();
    }
  }
  TemporaryObjectLocationMap.Empty();
  PrimaryActorTick.SetTickFunctionEnable(false);
  IsPlacing = false;
  LastLocation = FVector(-1, -1, -1);
}

//////////////////////////////////////////////////////////////////////////
AObjectBase* AObjectPlacer::GetTemporaryObject(FVector Location) {
  if (auto Object = TemporaryObjectLocationMap.Find(Location)) {
    return *Object;
  }
  return nullptr;
}
