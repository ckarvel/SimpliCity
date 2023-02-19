// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "MarkerComponent.generated.h"


UCLASS(Blueprintable,ClassGroup=(Custom),meta=(BlueprintSpawnableComponent))
class MARKERSYSTEM_API UMarkerComponent : public USceneComponent {
  GENERATED_BODY()

public:
  UMarkerComponent();

#if WITH_EDITORONLY_DATA
  UPROPERTY(EditAnywhere,Category = "MarkerComponent")
    bool visualizeComponent;
#endif

protected:
  virtual void BeginPlay() override;

#if WITH_EDITOR
  virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:
  UFUNCTION(BlueprintCallable,Category = "MarkerComponent")
  void AddAdjacentMarker(UMarkerComponent* Marker);

  UFUNCTION(BlueprintCallable,Category = "MarkerComponent")
  TArray<FVector> GetAdjacentMarkerLocations();

  UFUNCTION(BlueprintCallable,Category = "MarkerComponent")
  TArray<UMarkerComponent*> GetAdjacentMarkers() { return AdjacentMarkers; }

  UFUNCTION(BlueprintCallable,Category = "MarkerComponent")
  bool IsSourceMarker();

  UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "MarkerComponent")
  bool openForConnections;

  // 1 is Right lane
  // 0 can be right or left (turns/sidewalks)
  // -1 is Left lane
  UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "MarkerComponent")
  int laneDirection;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "MarkerComponent")
  TArray<UMarkerComponent*> AdjacentMarkers;

  UFUNCTION(BlueprintCallable,Category = "MarkerComponent")
  bool IsOpen() const { return openForConnections; }

  UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "MarkerComponent")
  bool canCurve; // can curve from this marker or is part of curved turn
};
