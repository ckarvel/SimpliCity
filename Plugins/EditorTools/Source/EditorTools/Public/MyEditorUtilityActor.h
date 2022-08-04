// Property of Carina Karvel

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityActor.h"
#include "MyEditorUtilityActor.generated.h"

/**
 * 
Provide many lines to draw - faster than calling DrawLine many times.
ENGINE_API void DrawLines(const TArray<FBatchedLine>& InLines);

Draw a box
ENGINE_API void DrawBox(const FBox& Box,const FMatrix& TM,const FColor& Color,uint8 InDepthPriorityGroup);

Draw an arrow
ENGINE_API void DrawDirectionalArrow(const FMatrix& ArrowToWorld,FColor InColor,float Length,float ArrowSize,uint8 DepthPriority);

Draw a circle
ENGINE_API void DrawCircle(const FVector& Base,const FVector& X,const FVector& Y,FColor Color,float Radius,int32 NumSides,uint8 DepthPriority);

ENGINE_API virtual void DrawLine(
	const FVector& Start,
	const FVector& End,
	const FLinearColor& Color,
	uint8 DepthPriority,
	float Thickness = 0.0f,
	float LifeTime = 0.0f
);
ENGINE_API virtual void DrawPoint(
	const FVector& Position,
	const FLinearColor& Color,
	float PointSize,
	uint8 DepthPriority,
	float LifeTime = 0.0f
);
**/
UCLASS(Blueprintable)
class AMyEditorUtilityActor : public AEditorUtilityActor
{
	GENERATED_BODY()
public:
	AMyEditorUtilityActor();
	UFUNCTION(BlueprintCallable)
	void DrawLine(
		const FVector& Start,
		const FVector& End,
		const FLinearColor& Color,
		float Thickness,
		float LifeTime,
		uint8 DepthPriority = 0
	);

	UFUNCTION(BlueprintCallable)
		void DrawPoint(
			const FVector& Position,
			const FLinearColor& Color,
			float PointSize,
			uint8 DepthPriority,
			float LifeTime = 0.0f
		);

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = Components,meta = (AllowPrivateAccess = "true"))
		class ULineBatchComponent* LineBatchComponent;
};
