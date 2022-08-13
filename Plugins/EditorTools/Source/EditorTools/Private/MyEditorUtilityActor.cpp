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

void AMyEditorUtilityActor::DrawArrow(const FVector& Start,
	const FVector& End,
	float ArrowSize,
	FColor const& Color,
	float LifeTime,
	uint8 DepthPriority,
	float Thickness) {
	if (ArrowSize <= 0) {
		ArrowSize = 10.f;
	}

	DrawLine(Start,End,FLinearColor(Color),Thickness,LifeTime,DepthPriority);

	FVector Dir = (End-Start);
	Dir.Normalize();
	FVector Up(0,0,1);
	FVector Right = Dir ^ Up;
	if (!Right.IsNormalized()) {
		Dir.FindBestAxisVectors(Up,Right);
	}
	FVector Origin = FVector::ZeroVector;
	FMatrix TM;
	// get matrix with dir/right/up
	TM.SetAxes(&Dir,&Right,&Up,&Origin);

	// since dir is x direction, my arrow will be pointing +y, -x and -y, -x
	float ArrowSqrt = FMath::Sqrt(ArrowSize);
	FVector ArrowPos;
	DrawLine(End,End + TM.TransformPosition(FVector(-ArrowSqrt,ArrowSqrt,0)),FLinearColor(Color),Thickness,LifeTime,DepthPriority);
	DrawLine(End,End + TM.TransformPosition(FVector(-ArrowSqrt,-ArrowSqrt,0)),FLinearColor(Color),Thickness,LifeTime,DepthPriority);
}