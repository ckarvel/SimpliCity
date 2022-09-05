// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpliCityFunctionLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "GridManager.h"
#include "MarkerManager.h"
#include "MyAStarPathFinder.h"

#include "SimpliCityGameInstance.h"
#include "SimpliCityObjectManager.h"
#include "SimpliCityPlayerController.h"
#include "SimpliCityMainUI.h"

#include "Road/SimpliCityRoadManager.h"
#include "Zone/SimpliCityZoneManager.h"

AGridManager* USimpliCityFunctionLibrary::GetGridManager(const UObject* WorldContextObject) {
  //USimpliCityGameInstance* gameInst = Cast<USimpliCityGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
  //check(gameInst != nullptr)
  //return gameInst->GetGridManager();
  auto mgr = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(WorldContextObject,AGridManager::StaticClass()));
  check(mgr != nullptr);
  return mgr;
}

ASimpliCityPlayerController* USimpliCityFunctionLibrary::GetPlayerController(const UObject* WorldContextObject) {
  auto mgr = Cast<ASimpliCityPlayerController>(UGameplayStatics::GetPlayerController(WorldContextObject, 0));
  check(mgr != nullptr);
  return mgr;
}

USimpliCityMainUI* USimpliCityFunctionLibrary::GetMainUI(const UObject* WorldContextObject) {
  ASimpliCityPlayerController* controller = GetPlayerController(WorldContextObject);
  auto mui = controller->TheMainUI;
  check(mui != nullptr);
  return mui;
}

ASimpliCityObjectManager* USimpliCityFunctionLibrary::GetObjectManager(const UObject* WorldContextObject) {
  auto mgr = Cast<ASimpliCityObjectManager>(UGameplayStatics::GetActorOfClass(WorldContextObject,ASimpliCityObjectManager::StaticClass()));
  check(mgr != nullptr);
  return mgr;
}

AMarkerManager* USimpliCityFunctionLibrary::GetMarkerManager(const UObject* WorldContextObject) {
  auto mgr = Cast<AMarkerManager>(UGameplayStatics::GetActorOfClass(WorldContextObject,AMarkerManager::StaticClass()));
  check(mgr != nullptr);
  return mgr;
}

//ASimpliCityRoadManager* USimpliCityFunctionLibrary::GetRoadManager(const UObject* WorldContextObject) {
//  auto mgr = Cast<ASimpliCityRoadManager>(UGameplayStatics::GetActorOfClass(WorldContextObject,ASimpliCityRoadManager::StaticClass()));
//  check(mgr != nullptr);
//  return mgr;
//}

ASimpliCityRoadManager* USimpliCityFunctionLibrary::GetRoadManager(const UObject* WorldContextObject) {
  return GetManager<ASimpliCityRoadManager>(WorldContextObject,ASimpliCityRoadManager::StaticClass());
}


//ASimpliCityZoneManager* USimpliCityFunctionLibrary::GetZoneManager(const UObject* WorldContextObject) {
//  auto mgr = Cast<ASimpliCityZoneManager>(UGameplayStatics::GetActorOfClass(WorldContextObject,ASimpliCityZoneManager::StaticClass()));
//  check(mgr != nullptr);
//  return mgr;
//}

ASimpliCityZoneManager* USimpliCityFunctionLibrary::GetZoneManager(const UObject* WorldContextObject) {
  return GetManager<ASimpliCityZoneManager>(WorldContextObject,ASimpliCityZoneManager::StaticClass());
}

template <class ManagerClass>
ManagerClass* USimpliCityFunctionLibrary::GetManager(const UObject* WorldContextObject, TSubclassOf<ManagerClass> Class) {
  ManagerClass* mgr = Cast<ManagerClass>(UGameplayStatics::GetActorOfClass(WorldContextObject,Class));
  check(mgr != nullptr);
  return mgr;
}

bool USimpliCityFunctionLibrary::AreLocationsEqual(FVector LocationA, FVector LocationB, float Tolerance) {
  FVector ZAdjustedA = FVector(LocationA.X,LocationA.Y,0.0);
  FVector ZAdjustedB = FVector(LocationB.X,LocationB.Y,0.0);
  return ZAdjustedA.Equals(ZAdjustedB,Tolerance);
}

TArray<FVector> USimpliCityFunctionLibrary::GetPathBetween(UObject* Graph,FVector Start,FVector End) {
  check(Graph != nullptr);
  TArray<FVector> newPath = MyAStarPathFinder::AStarSearch(Graph,Start,End);
  return newPath;
}

FVector USimpliCityFunctionLibrary::VInterpTo(FVector Current,FVector Target,float Delta,float Speed) {
  return FMath::VInterpTo(Current,Target,Delta,Speed);
}

FRotator USimpliCityFunctionLibrary::RInterpTo(FRotator Current, FRotator Target,float Delta,float Speed) {
  return FMath::RInterpTo(Current,Target,Delta,Speed);
}

float USimpliCityFunctionLibrary::FInterpTo(float Current,float Target,float Delta,float Speed) {
  return FMath::FInterpTo(Current,Target,Delta,Speed);
}

bool USimpliCityFunctionLibrary::IsNearlyEqual(float A,float B) {
  return FMath::IsNearlyEqual(A, B, 0.1);
}

void USimpliCityFunctionLibrary::CalculateSelectionRectangle(FVector Start,FVector End,FVector& OutExtents,TArray<FVector>& OutVertices,TArray<int>& OutTriangles) {
  float minX = FMath::Min(Start.X,End.X);
  float minY = FMath::Min(Start.Y,End.Y);
  float maxX = FMath::Max(Start.X,End.X);
  float maxY = FMath::Max(Start.Y,End.Y);

  TArray<FVector> Vertices = {
    FVector(minX, minY, 50),
    FVector(minX, maxY, 50),
    FVector(maxX, minY, 50),
    FVector(maxX, maxY, 50)
  };
  OutVertices = Vertices;

  OutExtents.X = FVector::Distance(Vertices[0],Vertices[2]) / 2.0;
  OutExtents.Y = FVector::Distance(Vertices[0],Vertices[3]) / 2.0;
  OutExtents.Z = 50.0; //arbitrary

  // order of vertices we made in the list above
  // counter-clockwise of a triangle starting at top
  TArray<int> Triangles = {0, 1, 3, 3, 2, 0};
  OutTriangles = Triangles;
}

FVector USimpliCityFunctionLibrary::GetMidpointBetween(FVector A,FVector B) {
  float x = (A.X + B.X) / 2.0;
  float y = (A.Y + B.Y) / 2.0;
  return FVector(x,y,50.0);
}

TArray<AActor*> USimpliCityFunctionLibrary::GetDifferenceInArrays(TArray<AActor*> A,TArray<AActor*> B) {
  TSet<AActor*> ASet = TSet<AActor*>(A);
  TSet<AActor*> BSet = TSet<AActor*>(B);
  TSet<AActor*> diff = ASet.Difference(BSet);
  return diff.Array();
}