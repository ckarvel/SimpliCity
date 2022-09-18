// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "SimpliCityInteractionMode.h"
#include "GameFramework/PlayerController.h"
#include "SimpliCityPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMouseUp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMouseCancel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMouseHover,FVector, HitLocation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMouseClick,FVector,HitLocation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMouseHold,FVector,HitLocation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnArrowInput,FVector,HitLocation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMiddleMouseClick,FVector,HitLocation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMiddleMouseHold,FRotator,Delta);

UCLASS(Blueprintable)
class ASimpliCityPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASimpliCityPlayerController();
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
	FOnMiddleMouseHold OnMiddleMouseHold;

	UFUNCTION(BlueprintCallable,Category = "SimpliCityPlayerController")
	FVector GetHitLocation() const { return TickHitLocation; }

	UFUNCTION(BlueprintCallable,Category = "SimpliCityPlayerController")
	void ResetBindings();

	// Build User Interface
	//UPROPERTY(VisibleAnywhere,Category = "SimpliCityPlayerController")
	//class USimpliCityMainUI* TheMainUI;

protected:
	void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

	void ClearBindings();

	void HandleInputEvents(bool blockingHit,FVector location);

	class ASimpliCityCharacter* ThePlayer;
	FVector TickHitLocation;
};
