// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "SimpliCityPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMouseUp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMouseCancel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMouseHover,FVector, HitLocation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMouseClick,FVector,HitLocation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMouseHold,FVector,HitLocation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnArrowInput,FVector,HitLocation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMiddleMouseClick,FRotator, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBuildMode,bool, IsEnabled);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnZoneMode,bool,IsEnabled);

UCLASS(Blueprintable)
class ASimpliCityPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASimpliCityPlayerController();

	//UFUNCTION(BlueprintCallable,Category = Bindings)
	//	void ClearBindings();
	//UFUNCTION(BlueprintCallable,Category = Bindings)
	//	void ClearAllBindings();
	
	UPROPERTY(BlueprintAssignable,Category = "SimpliCityPlayerController")
	FOnMouseHover OnMouseHover;
	UPROPERTY(BlueprintAssignable,Category = "SimpliCityPlayerController")
	FOnMouseClick OnMouseClick;
	UPROPERTY(BlueprintAssignable,Category = "SimpliCityPlayerController")
	FOnMouseHold OnMouseHold;
	UPROPERTY(BlueprintAssignable,Category = "SimpliCityPlayerController")
	FOnArrowInput OnArrowInput;
	UPROPERTY(BlueprintAssignable,Category = "SimpliCityPlayerController")
	FOnMouseUp OnMouseUp;
	UPROPERTY(BlueprintAssignable,Category = "SimpliCityPlayerController")
	FOnMouseCancel OnMouseCancel;
	UPROPERTY(BlueprintAssignable,Category = "SimpliCityPlayerController")
	FOnMiddleMouseClick OnMiddleMouseClick;
	UPROPERTY(BlueprintAssignable,Category = "SimpliCityPlayerController")
	FOnBuildMode OnBuildMode;
	UPROPERTY(BlueprintAssignable,Category = "SimpliCityPlayerController")
	FOnZoneMode OnZoneMode;

protected:
	void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

	void HandleHoverEvent(bool blockingHit,FVector location);
	void HandleClickEvent(bool blockingHit,FVector location);
	void HandleArrowEvent();

	UFUNCTION(BlueprintCallable, Category = "SimpliCityPlayerController")
	void EnableZoneMode();
	UFUNCTION(BlueprintCallable,Category = "SimpliCityPlayerController")
	void EnableBuildMode();

	bool BuildModeEnabled;
	bool ZoneModeEnabled;
};
