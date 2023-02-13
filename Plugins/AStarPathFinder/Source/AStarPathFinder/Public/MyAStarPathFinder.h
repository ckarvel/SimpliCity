// Property of Carina Karvel

#pragma once

#include "AStarPathFinder.h"
#include "PriorityQueueImpl.h"
#include "PathFinderInterface.h"
#include "Algo/Reverse.h"

struct MyAStarData {
	int loops = -1;
	TArray<FVector> path;
	bool error = false;
	FString message = "";
};

class MyAStarPathFinder
{
public:
	// Sets default values for this actor's properties
	MyAStarPathFinder() {};

	// distance between two points
	template <typename Element>
	static inline double heuristic(Element a,Element b) {
		return FMath::Abs(a.X - b.X) + FMath::Abs(a.Y - b.Y);
	}

	// creates a list of locations given a from-to node list (like a ghetto linked list)
	static MyAStarData reconstructPath(FVector Start, FVector End, TMap<FVector,FVector> Path) {
		TArray<FVector> NewPath;
		FVector Current = End;
		MyAStarData ReturnData;
		int loops = 0;
		while (Current.Equals(Start, 1) == false) {  // note: this will fail if no path found
			// keep track of loop count
			// avoid infinite loop
			ReturnData.loops = loops;

			if (loops >= 100) {
				ReturnData.message = "ERROR: (reconstructPath) Loop count >= 100!";
				ReturnData.error = true;
				return ReturnData;
			}
			NewPath.Add(Current);
			if (Path.Contains(Current)) {
				Current = Path.FindRef(Current);
			}
			loops+=1;
		}
		NewPath.Add(Start); // optional
		Algo::Reverse(NewPath);
		ReturnData.path = NewPath;
		return ReturnData;
	}

	// assumes start/end are valid!
	static MyAStarData AStarSearch(UObject* Graph, FVector Start, FVector End) {
		MyAStarData ReturnData;
		if (Graph->Implements<UPathFinderInterface>() == false) {
			ReturnData.message = "ERROR!! Graph->Implements<UPathFinderInterface>() == false";
			ReturnData.error = true;
			return ReturnData;
		}
		TPriorityQueue<FVector> Nodes;
		TArray<TPriorityQueueNode<FVector>> Neighbors;
		TMap<FVector,FVector> PathMap;
		TMap<FVector,double> Cost;

		Cost.Add(Start,0);
		Nodes.Push(Start,0);

		int loops = 0;
		FVector Current;
		while (Nodes.IsEmpty() == false) {
			// keep track of loop count
			// avoid infinite loop
			ReturnData.loops = loops;

			if (loops >= 100) {
				ReturnData.message = "ERROR: Loop count >= 100!";
				ReturnData.error = true;
				return ReturnData;
			}

			Current = Nodes.Pop();
			
			if (Current.Equals(End, 1)) {
				break;
			}

			IPathFinderInterface* PathFinder = Cast<IPathFinderInterface>(Graph);
			TArray<FVector> NeighborList = PathFinder->GetNeighbors(Current);

			// ------------------------
			// handle error gracefully
			if (NeighborList.IsEmpty()) {
				ReturnData.message = "WARNING: NeighborList is empty";
				ReturnData.error = true;
				return ReturnData;
			}
			// ------------------------

			for (FVector Next : NeighborList) {
				double new_cost = Cost.FindRef(Current) + PathFinder->GetCost(Next);
				if (Cost.Contains(Next) == false || new_cost < Cost.FindRef(Next)) {
					Cost.Add(Next,new_cost);
					double priority = new_cost + heuristic(Next,End);
					Nodes.Push(Next,priority);
					PathMap.Add(Next, Current);
				}
			}
			loops += 1;
		}

		if (!Current.Equals(End,1)) {
			ReturnData.message = "Warning: Couldn't find path";
			ReturnData.error = true;
			return ReturnData;
		}

		// sort nodes into a list of locations
		MyAStarData reconData = reconstructPath(Start, End, PathMap);
		ReturnData.path = reconData.path;
		if (reconData.error) {
			ReturnData = reconData;
		}
		return ReturnData;
	}
};
