#pragma once
#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <iostream>
#include <vector> // Used only for returning the path/routing table
#include "LinkedList.hpp"
#include "BinarySearchTable.hpp"

// Forward declaration
struct TVertex;

/**
 * @brief Represents a weighted connection to another city.
 */
struct TEdge {
	TVertex* destination;
	float weight;
	TEdge* next; // Linked list of edges

	TEdge(TVertex* aDest, float aWeight)
		: destination(aDest), weight(aWeight), next(nullptr) {
	}
};

/**
 * @brief Represents a City in the network.
 */
struct TVertex {
	std::string name;
	TEdge* edges; // Head of the adjacency list

	// --- Dijkstra Helper Fields ---
	float minDistance;
	TVertex* previous; // For path reconstruction
	bool visited;

	TVertex(std::string aName)
		: name(aName), edges(nullptr), minDistance(1e9), previous(nullptr), visited(false) {
	}

	// Helper to add an edge to this vertex's list
	void AddEdge(TVertex* aDestination, float aWeight) {
		TEdge* newEdge = new TEdge(aDestination, aWeight);
		newEdge->next = edges;
		edges = newEdge;
	}
};

/**
 * @brief Adjacency List Graph with Dijkstra capabilities.
 */
class TGraph {
private:
	// 1. Data Ownership: Use a Linked List to hold all vertices so we can delete them.
	TLinkedList<TVertex*> allVertices;

	// 2. Fast Lookup: Map "CityName" -> TVertex*
	TBinarySearchTable<TVertex*> vertexLookup;

	// 3. Count of vertices (useful for init PriorityQueue)
	int vertexCount;

public:
	TGraph();
	~TGraph();

	// --- Building the Graph ---
	/**
	 * @brief Creates a vertex if it doesn't exist.
	 */
	TVertex* CreateVertex(const std::string& aName);

	/**
	 * @brief Adds a directed weighted edge.
	 */
	void AddEdge(const std::string& aFrom, const std::string& aTo, float aWeight);

	// --- Algorithms ---
	/**
	 * @brief Resets all vertices to infinite distance and unvisited state.
	 */
	void ResetState();

	/**
	 * @brief Runs Dijkstra's algorithm from a start node.
	 * Computes the shortest path to ALL other nodes.
	 */
	bool RunDijkstra(const std::string& aStartCity);

	/**
	 * @brief Prints the routing table (Cost from Start -> All Cities).
	 */
	void PrintRoutingTable() const;

	// Helper to check if graph is empty
	bool IsEmpty() const { return vertexCount == 0; }

	/**
		 * @brief Prints a list of all available cities (Vertices).
		 */
	void PrintVertices() const;
};

#endif // GRAPH_H