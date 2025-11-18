#include <iostream>
#include "option1.h"
#include "SharedLib.h"
static constexpr std::string_view AssignmentOption = "Option 1 (Standard): Data Center Network Monitor.";


/**
 * @brief Callback function to process one node.
 */
static bool NodeReadCallback(const int aIndex, const int aTotalCount, const std::string& aNode)
{
	std::cout << "Loading Node " << (aIndex + 1) << " of " << aTotalCount << ": " << aNode << "\n";

	// Return true to continue reading
	return true;
}

/**
 * @brief Callback function to process one edge.
 */
static bool EdgeReadCallback(const int aIndex, const int aTotalCount, const std::string& aFromNode, const std::string& aToNode, float aWeight)
{
	std::cout << "  Loading Edge " << (aIndex + 1) << " of " << aTotalCount << ": "
		<< aFromNode << " -> " << aToNode << " (Weight: " << aWeight << ")\n";

	// Return true to continue reading
	return true;
}


int RunApp()
{
	// Path to the graph data file
	std::string filename = "F:\\IKT203\\VisualStudio\\DATA\\city_graph.txt";

	std::cout << "Reading graph from file: " << filename << "\n\n";

	// Call the utility function with both callbacks
	readGraphFromFile(filename, NodeReadCallback, EdgeReadCallback);

	std::cout << "\nFinished reading graph." << std::endl;

	return 0;
}