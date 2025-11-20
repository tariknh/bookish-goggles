#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "SharedLib.h"
#include "FileReaderUtils.h"

// --- Enum for the parser's state ---
enum class EParseState
{
	NONE,
	NODES,
	EDGES
};

void readGraphFromFile(const std::string& aFilename, FNodeRead aOnNodeRead, FEdgeRead aOnEdgeRead)
{
	if (aFilename.empty()) return;

	std::ifstream file(aFilename);
	if (!file.is_open())
	{
		// Optional: print an error
		// std::cerr << "Error: Could not open file " << aFilename << std::endl;
		return;
	}

	std::string line;
	EParseState currentState = EParseState::NONE;
	int totalCount = 0;
	int currentIndex = 0;
	bool keepReading = true;

	while (keepReading && std::getline(file, line))
	{
		if (line.empty()) continue;

		if (line[0] == '[')
		{
			// --- 2. USE THE SHARED FUNCTION ---
			totalCount = GetRecordCount(line);
			currentIndex = 0;

			if (line.find("[NODES") != std::string::npos)
			{
				currentState = EParseState::NODES;
				continue;
			}
			else if (line.find("[EDGES") != std::string::npos)
			{
				currentState = EParseState::EDGES;
				continue;
			}
			// If it's a comment or other header, reset state and count
			currentState = EParseState::NONE;
			totalCount = 0;
			continue;
		}

		// Process data based on the current state
		switch (static_cast<int>(currentState))
		{
		case static_cast<int>(EParseState::NODES):
			if (aOnNodeRead)
			{
				if (!aOnNodeRead(currentIndex, totalCount, line))
				{
					keepReading = false;
				}
				currentIndex++;
			}
			break;

		case static_cast<int>(EParseState::EDGES):
		{
			std::istringstream edgeStream(line);
			std::string fromNode, toNode, weightStr;

			if (std::getline(edgeStream, fromNode, ';') &&
				std::getline(edgeStream, toNode, ';') &&
				std::getline(edgeStream, weightStr))
			{
				try
				{
					// Use std::stof (string to float) for weight
					float weight = std::stof(weightStr);
					if (aOnEdgeRead)
					{
						if (!aOnEdgeRead(currentIndex, totalCount, fromNode, toNode, weight))
						{
							keepReading = false;
						}
						currentIndex++;
					}
				}
				catch (const std::exception&)
				{
					// Failed to parse float, skip this line
				}
			}
			break;
		}
		case static_cast<int>(EParseState::NONE):
		default:
			break;
		}
	}
	file.close();
}