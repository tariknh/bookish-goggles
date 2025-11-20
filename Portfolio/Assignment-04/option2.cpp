#include "option2.h"
#include "SharedLib.h"
#include "Graph.h"
#include <iostream>
#include <string>

// --- Global / Static Instance for Callbacks ---
// We need this because the function pointers in SharedLib do not accept a context pointer.
static TGraph* gGraphInstance = nullptr;

// --- Callbacks ---

static bool NodeReadCallback(const int aIndex, const int aTotalCount, const std::string& aNode)
{
	if (gGraphInstance)
	{
		// Just ensure the vertex is created/registered
		gGraphInstance->CreateVertex(aNode);
	}
	return true;
}

static bool EdgeReadCallback(const int aIndex, const int aTotalCount, const std::string& aFrom, const std::string& aTo, float aWeight)
{
	if (gGraphInstance)
	{
		// Add the directed edge
		gGraphInstance->AddEdge(aFrom, aTo, aWeight);
	}
	return true;
}

// --- Main App ---

int RunApp()
{
	// 1. Initialize Graph
	TGraph graph;
	gGraphInstance = &graph; // Point the static global to our local instance

	std::cout << "Option 2 (Advanced): Inter-city Logistics Router.\n";

	// 2. Load Data
	// Note: Update this path if your data is elsewhere. 
	// The assignment requires using "city_graph.txt" which is a Directed Graph.
	std::string filename = "F:\\IKT203\\VisualStudio\\DATA\\city_graph.txt";

	std::cout << "Reading graph from: " << filename << " ...\n";
	readGraphFromFile(filename, NodeReadCallback, EdgeReadCallback);

	if (graph.IsEmpty()) {
		std::cout << "Error: Graph is empty. Check file path.\n";
		return 1;
	}
	std::cout << "Graph Loaded Successfully.\n\n";

	graph.PrintVertices();

	// 3. User Interaction Loop
	std::string input;
	while (true)
	{
		std::cout << "Enter Start City (or 'exit'): ";
		if (!std::getline(std::cin, input) || input == "exit")
		{
			break;
		}
		if (input.empty()) continue;

		// 4. Run Algorithm
		if (graph.RunDijkstra(input))
		{
			// 5. Display Results
			graph.PrintRoutingTable();
		}
	}

	// Reset static pointer
	gGraphInstance = nullptr;
	return 0;
}