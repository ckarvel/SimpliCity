// Property of Carina Karvel


#include "MyEditorUtilityActor.h"
#include "Components/LineBatchComponent.h"

AMyEditorUtilityActor::AMyEditorUtilityActor() {
	LineBatchComponent = CreateDefaultSubobject<ULineBatchComponent>("LineBatch");
}

void AMyEditorUtilityActor::DrawLine(const FVector& Start,
	const FVector& End,
	const FLinearColor& Color,
	uint8 DepthPriority,
	float Thickness,
	float LifeTime) {
	if (LineBatchComponent) {
		LineBatchComponent->DrawLine(Start,End,Color,0,Thickness,LifeTime);
	}
}