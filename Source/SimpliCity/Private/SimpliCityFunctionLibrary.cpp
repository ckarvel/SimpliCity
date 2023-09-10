// Fill out your copyright notice in the Description page of Project Settings.

#include "SimpliCityFunctionLibrary.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "GridManager.h"
#include "MarkerManager.h"
#include "MyAStarPathFinder.h"

#include "SimpliCityGameInstance.h"
#include "ObjectManager.h"
#include "ObjectPlacer.h"
#include "SimpliCityObjectSelector.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Getters
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
AGridManager* USimpliCityFunctionLibrary::GetGridManager(const UObject* WorldContextObject) {
  return GetManager<AGridManager>(WorldContextObject, AGridManager::StaticClass());
}

//////////////////////////////////////////////////////////////////////////
AObjectManager* USimpliCityFunctionLibrary::GetObjectManager(const UObject* WorldContextObject) {
  return GetManager<AObjectManager>(WorldContextObject, AObjectManager::StaticClass());
}

//////////////////////////////////////////////////////////////////////////
AObjectPlacer* USimpliCityFunctionLibrary::GetObjectPlacer(const UObject* WorldContextObject) {
  return GetManager<AObjectPlacer>(WorldContextObject, AObjectPlacer::StaticClass());
}

//////////////////////////////////////////////////////////////////////////
AMarkerManager* USimpliCityFunctionLibrary::GetMarkerManager(const UObject* WorldContextObject) {
  return GetManager<AMarkerManager>(WorldContextObject, AMarkerManager::StaticClass());
}

//////////////////////////////////////////////////////////////////////////
template <class ManagerClass>
ManagerClass* USimpliCityFunctionLibrary::GetManager(const UObject* WorldContextObject,
                                                     TSubclassOf<ManagerClass> Class) {
  ManagerClass* mgr = Cast<ManagerClass>(UGameplayStatics::GetActorOfClass(WorldContextObject, Class));
  if (mgr == nullptr) {
    TRACE_ERROR_PRINTF(LogSimpliCity, "ERROR!! mgr == nullptr");
  }
  return mgr;
}

//////////////////////////////////////////////////////////////////////////
ASimpliCityObjectSelector* USimpliCityFunctionLibrary::GetSelector(const UObject* WorldContextObject) {
  ASimpliCityObjectSelector* selector = Cast<ASimpliCityObjectSelector>(
      UGameplayStatics::GetActorOfClass(WorldContextObject, ASimpliCityObjectSelector::StaticClass()));
  if (selector == nullptr) {
    TRACE_ERROR_PRINTF(LogSimpliCity, "ERROR!! selector == nullptr");
  }
  return selector;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Common Math Functions
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool USimpliCityFunctionLibrary::IsNearlyEqual(float A, float B) {
  return FMath::IsNearlyEqual(A, B, 0.1);
}

//////////////////////////////////////////////////////////////////////////
bool USimpliCityFunctionLibrary::AreLocationsEqual(FVector LocationA, FVector LocationB, float Tolerance) {
  FVector ZAdjustedA = FVector(LocationA.X, LocationA.Y, 0.0);
  FVector ZAdjustedB = FVector(LocationB.X, LocationB.Y, 0.0);
  return ZAdjustedA.Equals(ZAdjustedB, Tolerance);
}

//////////////////////////////////////////////////////////////////////////
FVector USimpliCityFunctionLibrary::GetMidpointBetween(FVector A, FVector B) {
  float x = (A.X + B.X) / 2.0;
  float y = (A.Y + B.Y) / 2.0;
  return FVector(x, y, 50.0);
}

//////////////////////////////////////////////////////////////////////////
FVector USimpliCityFunctionLibrary::VInterpTo(FVector Current, FVector Target, float Delta, float Speed) {
  return FMath::VInterpTo(Current, Target, Delta, Speed);
}

//////////////////////////////////////////////////////////////////////////
FRotator USimpliCityFunctionLibrary::RInterpTo(FRotator Current, FRotator Target, float Delta, float Speed) {
  return FMath::RInterpTo(Current, Target, Delta, Speed);
}

//////////////////////////////////////////////////////////////////////////
float USimpliCityFunctionLibrary::FInterpTo(float Current, float Target, float Delta, float Speed) {
  return FMath::FInterpTo(Current, Target, Delta, Speed);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Math Algorithms
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TArray<AActor*> USimpliCityFunctionLibrary::GetDifferenceInArrays(TArray<AActor*> A, TArray<AActor*> B) {
  TSet<AActor*> ASet = TSet<AActor*>(A);
  TSet<AActor*> BSet = TSet<AActor*>(B);
  TSet<AActor*> diff = ASet.Difference(BSet);
  return diff.Array();
}

//////////////////////////////////////////////////////////////////////////
void USimpliCityFunctionLibrary::CalculateSelectionRectangle(FVector Start, FVector End, FVector& OutExtents,
                                                             TArray<FVector>& OutVertices, TArray<int>& OutTriangles) {
  float minX = FMath::Min(Start.X, End.X);
  float minY = FMath::Min(Start.Y, End.Y);
  float maxX = FMath::Max(Start.X, End.X);
  float maxY = FMath::Max(Start.Y, End.Y);

  TArray<FVector> Vertices = {FVector(minX, minY, 100), FVector(minX, maxY, 100), FVector(maxX, minY, 100),
                              FVector(maxX, maxY, 100)};
  OutVertices = Vertices;

  OutExtents.X = FVector::Distance(Vertices[0], Vertices[2]) / 2.0;
  OutExtents.Y = FVector::Distance(Vertices[0], Vertices[1]) / 2.0;
  OutExtents.Z = 50.0; // arbitrary

  // order of vertices we made in the list above
  // counter-clockwise of a triangle starting at top
  TArray<int> Triangles = {0, 1, 3, 3, 2, 0};
  OutTriangles = Triangles;
}

//////////////////////////////////////////////////////////////////////////
TArray<FVector> USimpliCityFunctionLibrary::GetPathBetween(UObject* Graph, FVector Start, FVector End) {
  if (Graph == nullptr) {
    TRACE_ERROR_PRINTF(LogSimpliCity, "ERROR!! Graph == nullptr");
    return TArray<FVector>();
  }
  MyAStarData Data = MyAStarPathFinder::AStarSearch(Graph, Start, End);
  if (Data.error == true) {
    TRACE_ERROR_PRINTF(LogSimpliCity, "%s", *Data.message);
  }
  return Data.path;
}

//////////////////////////////////////////////////////////////////////////
bool USimpliCityFunctionLibrary::ArePointsCollinear(FVector A, FVector B, FVector C) {
  // (x2-x1)(y3-y2) - (y2-y1)(x3-x2) = 0
  float result = (B.X - A.X) * (C.Y - B.Y) - (B.Y - A.Y) * (C.X - B.X);
  return result == 0;
}

//////////////////////////////////////////////////////////////////////////
TArray<FVector> USimpliCityFunctionLibrary::GetSimplifiedPath(TArray<FVector> Path) {
  if (Path.Num() < 3) {
    TRACE_WARNING_PRINTF(LogSimpliCity, "Warning: Path has less than 3 points");
    return Path;
  }

  TArray<FVector> Out = Path;
  int start = 0;
  while (start + 2 < Out.Num()) {
    if (USimpliCityFunctionLibrary::ArePointsCollinear(Out[start], Out[start + 1], Out[start + 2])) {
      Out.RemoveAt(start + 1);
      continue;
    }
    start++;
  }
  return Out;
}

//////////////////////////////////////////////////////////////////////////
float USimpliCityFunctionLibrary::AngleBetween2Vectors(FVector A, FVector B) {
  A.Normalize();
  B.Normalize();
  float dot = FVector::DotProduct(A, B);
  float rads = FMath::Acos(dot);
  return FMath::RadiansToDegrees(rads);
}

//////////////////////////////////////////////////////////////////////////
TArray<FVector> USimpliCityFunctionLibrary::QuadraticBezierCurve(FVector P0, FVector P1, FVector P2) {
  // B(t) = (1-t)^2 * P0 + 2(1-t)tP1 + t2P2,0 <= t <= 1
  float t = 0;
  int num_points = 10;
  float step_size = 1.0f / num_points;
  TArray<FVector> OutPoints;
  while (t <= 1) {
    FVector C0 = FMath::Square(1 - t) * P0;
    FVector C1 = 2 * t * (1 - t) * P1;
    FVector C2 = FMath::Square(t) * P2;
    OutPoints.Add(C0 + C1 + C2);
    t += step_size;
  }
  return OutPoints;
}

//////////////////////////////////////////////////////////////////////////
TArray<FVector> USimpliCityFunctionLibrary::SmoothCurvedSegments(TArray<FVector> Path) {
  if (Path.Num() < 3) {
    TRACE_WARNING_PRINTF(LogSimpliCity, "Warning: Path has less than 3 points");
    return Path;
  }

  TArray<FVector> Out = Path;
  int start = 0;
  while (start + 2 < Out.Num()) {
    // Get angle between 2 vectors
    FVector A = Out[start + 2] - Out[start + 1];
    FVector B = Out[start + 1] - Out[start];
    float angle = USimpliCityFunctionLibrary::AngleBetween2Vectors(A, B);
    if (angle > 45) {
      TArray<FVector> CurvePoints =
          USimpliCityFunctionLibrary::QuadraticBezierCurve(Out[start], Out[start + 1], Out[start + 2]);
      Out.RemoveAt(start + 2);
      Out.RemoveAt(start + 1);
      Out.RemoveAt(start);
      Out.Insert(CurvePoints, start);
      // start should be set to the last point in this curve
      start += CurvePoints.Num() - 1;
      continue;
    }
    start++;
  }
  return Out;
}

//////////////////////////////////////////////////////////////////////////
// 0 - left, right, bottom (0, 1, 3)
// 90 - left, top, bottom (0, 2, 3)
// 180 - left, right, top (0, 1, 2)
// 270 - right, top, bottom (1, 2, 3)
float USimpliCityFunctionLibrary::Get3Way(TArray<int> ValidIndexes) {
  int neighbor1 = ValidIndexes[0];
  int neighbor2 = ValidIndexes[1];
  int neighbor3 = ValidIndexes[2];
  if (neighbor1 == 0 && neighbor2 == 1 && neighbor3 == 3) {
    return 0.0;
  }
  if (neighbor1 == 0 && neighbor2 == 2 && neighbor3 == 3) {
    return 90.0;
  }
  if (neighbor1 == 0 && neighbor2 == 1 && neighbor3 == 2) {
    return 180.0;
  }
  return 270.0;
}

//////////////////////////////////////////////////////////////////////////
// 0 - top, bottom (2, 3)
// 90 - left, right (0, 1)
float USimpliCityFunctionLibrary::Get2Way_Straight(TArray<int> ValidIndexes) {
  int neighbor1 = ValidIndexes[0];
  int neighbor2 = ValidIndexes[1];
  if (neighbor1 == 2 && neighbor2 == 3) {
    return 0.0;
  }
  if (neighbor1 == 0 && neighbor2 == 1) {
    return 90.0;
  }
  // HACK: this value used to determine if straight or curve mesh
  return 360; // curve
}

//////////////////////////////////////////////////////////////////////////
// 0 - right, top (1, 2)
// 90 - right, bottom (1, 3)
// 180 - left, bottom (0, 3)
// 270 - left, top (0, 2)
float USimpliCityFunctionLibrary::Get2Way_Curve(TArray<int> ValidIndexes) {
  int neighbor1 = ValidIndexes[0];
  int neighbor2 = ValidIndexes[1];
  if (neighbor1 == 1 && neighbor2 == 2) {
    return 0.0;
  }
  if (neighbor1 == 1 && neighbor2 == 3) {
    return 90.0;
  }
  if (neighbor1 == 0 && neighbor2 == 3) {
    return 180.0;
  }
  return 270.0;
}

//////////////////////////////////////////////////////////////////////////
// 0 - bottom (3)
// 90 - left (0)
// 180 - top (2)
// 270 - right (1)
float USimpliCityFunctionLibrary::Get1Way(TArray<int> ValidIndexes) {
  int neighbor1 = ValidIndexes[0];
  if (neighbor1 == 3) {
    return 0.0;
  }
  if (neighbor1 == 0) {
    return 90.0;
  }
  if (neighbor1 == 2) {
    return 180.0;
  }
  return 270.0;
}

//////////////////////////////////////////////////////////////////////////
bool USimpliCityFunctionLibrary::IsCornerMesh(TArray<int> ValidIndexes) {
  if (USimpliCityFunctionLibrary::Get2Way_Straight(ValidIndexes) == 360) {
    return true;
  }
  return false;
}

//////////////////////////////////////////////////////////////////////////
FRotator USimpliCityFunctionLibrary::GetRotation(TArray<int> ValidIndexes) {
  FRotator OutRotation = FRotator(0, 0, 0);
  float yaw = 0;
  int neighborCount = ValidIndexes.Num();
  if (neighborCount == 0) {
    return OutRotation;
  }
  switch (neighborCount) {
  case 4:
    yaw = 0.0;
    break;
  case 3:
    yaw = USimpliCityFunctionLibrary::Get3Way(ValidIndexes);
    break;
  case 2:
    if (USimpliCityFunctionLibrary::IsCornerMesh(ValidIndexes)) {
      yaw = USimpliCityFunctionLibrary::Get2Way_Curve(ValidIndexes);
    } else {
      yaw = USimpliCityFunctionLibrary::Get2Way_Straight(ValidIndexes);
    }
    break;
  default:
    yaw = USimpliCityFunctionLibrary::Get1Way(ValidIndexes);
    break;
  };
  OutRotation.Yaw = yaw;
  return OutRotation;
}