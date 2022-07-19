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
	
	UPROPERTY(BlueprintAssignable)
	FOnMouseHover OnMouseHover;
	UPROPERTY(BlueprintAssignable)
	FOnMouseClick OnMouseClick;
	UPROPERTY(BlueprintAssignable)
	FOnMouseHold OnMouseHold;
	UPROPERTY(BlueprintAssignable)
	FOnArrowInput OnArrowInput;
	UPROPERTY(BlueprintAssignable)
	FOnMouseUp OnMouseUp;
	UPROPERTY(BlueprintAssignable)
	FOnMouseCancel OnMouseCancel;
	UPROPERTY(BlueprintAssignable)
	FOnMiddleMouseClick OnMiddleMouseClick;

protected:
	void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

	void HandleHoverEvent(bool blockingHit,FVector location);
	void HandleClickEvent(bool blockingHit,FVector location);
	void HandleArrowEvent();

	//class ASimpliCityCharacter* ThePlayer;
};
