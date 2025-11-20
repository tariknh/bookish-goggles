#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "SharedLib.h"
#include "FileReaderUtils.h"

void readNamesFromFile(const std::string& aFilename, FNameRead aOnNameRead)
{
	if (aFilename.empty()) return;

	std::ifstream file(aFilename);
	if (!file.is_open())
	{
		std::cerr << "Error: Could not open file " << aFilename << std::endl;
		return;
	}

	std::string line;
	int totalCount = 0;
	int currentIndex = 0;
	bool keepReading = true;

	// --- 1. Read the header line ---
	if (std::getline(file, line))
	{
		// Use our shared helper to get the count
		totalCount = GetRecordCount(line);
	}

	// --- 2. Loop through the rest of the file ---
	while (keepReading && std::getline(file, line))
	{
		if (line.empty()) continue;

		std::istringstream nameStream(line);
		std::string firstName, lastName;

		// Parse "FirstName LastName"
		if (nameStream >> firstName >> lastName)
		{
			if (aOnNameRead)
			{
				// Call the callback with all parameters
				if (!aOnNameRead(currentIndex, totalCount, firstName, lastName))
				{
					keepReading = false;
				}
				currentIndex++;
			}
		}
	}

	file.close();
}