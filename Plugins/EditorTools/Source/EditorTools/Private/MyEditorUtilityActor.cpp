// Property of Carina Karvel


#include "MyEditorUtilityActor.h"
#include "Components/LineBatchComponent.h"

AMyEditorUtilityActor::AMyEditorUtilityActor() {
	LineBatchComponent = CreateDefaultSubobject<ULineBatchComponent>("LineBatch");
}

void AMyEditorUtilityActor::DrawLine(const FVector& Start,
	const FVector& End,
	const FLinearColor& Color,
	float Thickness,
	float LifeTime, 
	uint8 DepthPriority) {
	if (LineBatchComponent) {
		LineBatchComponent->DrawLine(Start,End,Color,DepthPriority,Thickness,LifeTime);
	}
}

void AMyEditorUtilityActor::DrawPoint(const FVector& Position,
	const FLinearColor& Color,
	float PointSize,
	uint8 DepthPriority,
	float LifeTime) {
	if (LineBatchComponent) {
		LineBatchComponent->DrawPoint(Position,Color,PointSize,DepthPriority,LifeTime);
	}
}