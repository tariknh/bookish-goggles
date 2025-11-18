#include "Graph.h"
#include "PriorityQueue.hpp"
#include <iostream>
#include <limits> // for infinity

TGraph::TGraph() : vertexCount(0)
{
	// The list will hold the vertices, but we will handle deletion manually
	// to ensure edges are deleted first.
}

TGraph::~TGraph()
{
	// 1. Iterate over all vertices to delete their Edges
	TLinkedListNode<TVertex*>* currentNode = allVertices.GetHead()->GetNext();

	while (currentNode != allVertices.GetTail())
	{
		TVertex* v = currentNode->GetData();

		// Delete the linked list of edges for this vertex
		TEdge* currentEdge = v->edges;
		while (currentEdge != nullptr)
		{
			TEdge* temp = currentEdge;
			currentEdge = currentEdge->next;
			delete temp;
		}

		// Now it is safe to delete the vertex itself
		delete v;

		currentNode = currentNode->GetNext();
	}

	// 2. The TLinkedList destructor will run, but we've already cleaned up the data.
	// (Assuming the list was initialized with isDataOwner = false, which is default)
}

TVertex* TGraph::CreateVertex(const std::string& aName)
{
	// 1. Check if it exists
	TVertex* existing = nullptr;
	if (vertexLookup.Search(aName, existing))
	{
		return existing;
	}

	// 2. Create new
	TVertex* newVertex = new TVertex(aName);

	// 3. Add to storage (List) and Lookup (BST)
	allVertices.Append(newVertex);
	vertexLookup.Insert(aName, newVertex);
	vertexCount++;

	return newVertex;
}

void TGraph::AddEdge(const std::string& aFrom, const std::string& aTo, float aWeight)
{
	// Ensure both vertices exist
	TVertex* fromV = CreateVertex(aFrom);
	TVertex* toV = CreateVertex(aTo);

	// Add the directed edge
	fromV->AddEdge(toV, aWeight);
}

void TGraph::ResetState()
{
	TLinkedListNode<TVertex*>* currentNode = allVertices.GetHead()->GetNext();
	while (currentNode != allVertices.GetTail())
	{
		TVertex* v = currentNode->GetData();
		v->minDistance = std::numeric_limits<float>::infinity();
		v->previous = nullptr;
		v->visited = false;

		currentNode = currentNode->GetNext();
	}
}

bool TGraph::RunDijkstra(const std::string& aStartCity)
{
	// 1. Find Start Node
	TVertex* startNode = nullptr;
	if (!vertexLookup.Search(aStartCity, startNode))
	{
		std::cerr << "Error: Start city '" << aStartCity << "' not found." << std::endl;
		return false;
	}

	// 2. Reset
	ResetState();

	// 3. Initialize Priority Queue
	// Capacity estimation: V * 4 is usually safe for edge relaxations in sparse graphs,
	// or we can just use a large number if we don't want to implement dynamic resizing.
	int pqCapacity = (vertexCount * 10) + 100;
	TPriorityQueue<TVertex*> pq(pqCapacity);

	// 4. Setup Start
	startNode->minDistance = 0.0f;
	pq.Enqueue(startNode, 0.0f);

	// 5. Process Loop
	while (!pq.IsEmpty())
	{
		TVertex* u = pq.Dequeue();

		// Optimization: If we extracted a node that was already visited/closed? 
		// Dijkstra with simple PQ can add multiple entries for same node.
		// We assume standard behavior here.

		// Iterate Neighbors (Edges)
		TEdge* e = u->edges;
		while (e != nullptr)
		{
			TVertex* v = e->destination;
			float weight = e->weight;

			// Relaxation Step
			float distanceThroughU = u->minDistance + weight;

			if (distanceThroughU < v->minDistance)
			{
				v->minDistance = distanceThroughU;
				v->previous = u;

				// Add to PQ
				pq.Enqueue(v, v->minDistance);
			}

			e = e->next;
		}
	}

	return true;
}

void TGraph::PrintRoutingTable() const
{
	std::cout << "\n--- Routing Table (Lowest Cost from Source) ---\n";

	TLinkedListNode<TVertex*>* currentNode = allVertices.GetHead()->GetNext();
	while (currentNode != allVertices.GetTail())
	{
		TVertex* v = currentNode->GetData();

		std::cout << "Destination: " << v->name;

		if (v->minDistance == std::numeric_limits<float>::infinity())
		{
			std::cout << " | Cost: [Unreachable]" << std::endl;
		}
		else
		{
			std::cout << " | Cost: " << v->minDistance << std::endl;
		}

		currentNode = currentNode->GetNext();
	}
	std::cout << "-----------------------------------------------\n";
}

// Inside TGraph.cpp

void TGraph::PrintVertices() const
{
	std::cout << "--- Available Cities ---\n";

	// Use the linked list we already have
	TLinkedListNode<TVertex*>* currentNode = allVertices.GetHead()->GetNext();
	int count = 0;

	while (currentNode != allVertices.GetTail())
	{
		TVertex* v = currentNode->GetData();
		std::cout << " - " << v->name << "\n";

		currentNode = currentNode->GetNext();
		count++;
	}
	std::cout << "------------------------\n";
	std::cout << "Total: " << count << " cities.\n\n";
}