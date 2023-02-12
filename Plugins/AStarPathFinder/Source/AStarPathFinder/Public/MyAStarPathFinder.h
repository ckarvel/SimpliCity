// Property of Carina Karvel

#pragma once

#include "AStarPathFinder.h"
#include "PriorityQueueImpl.h"
#include "PathFinderInterface.h"
#include "Algo/Reverse.h"

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
	static TArray<FVector> reconstructPath(FVector Start, FVector End, TMap<FVector,FVector> Path) {
		TArray<FVector> NewPath;
		FVector Current = End;
		while (Current.Equals(Start, 1) == false) {  // note: this will fail if no path found
			NewPath.Add(Current);
			if (Path.Contains(Current)) {
				Current = Path.FindRef(Current);
			}
		}
		NewPath.Add(Start); // optional
		Algo::Reverse(NewPath);
		return NewPath;
	}

	// assumes start/end are valid!
	static TArray<FVector> AStarSearch(UObject* Graph, FVector Start, FVector End) {
		if (Graph->Implements<UPathFinderInterface>() == false) {
			//TRACE_ERROR_PRINTF(FAStarPathFinderModule::LogAStarPathFinder,"ERROR!! Graph->Implements<UPathFinderInterface>() == false");
			return TArray<FVector>();
		}
		TPriorityQueue<FVector> Nodes;
		TArray<TPriorityQueueNode<FVector>> Neighbors;
		TMap<FVector,FVector> PathMap;
		TMap<FVector,double> Cost;

		Cost.Add(Start,0);
		Nodes.Push(Start,0);

		while (Nodes.IsEmpty() == false) {
			FVector Current = Nodes.Pop();
			
			if (Current.Equals(End, 1)) {
				break;
			}

			IPathFinderInterface* PathFinder = Cast<IPathFinderInterface>(Graph);
			TArray<FVector> NeighborList = PathFinder->GetNeighbors(Current);

			// ------------------------
			// handle error gracefully
			if (NeighborList.IsEmpty()) {
				return TArray<FVector>();
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
		}

		// sort nodes into a list of locations
		TArray<FVector> Path = reconstructPath(Start, End, PathMap);
		return Path;
	}
};
