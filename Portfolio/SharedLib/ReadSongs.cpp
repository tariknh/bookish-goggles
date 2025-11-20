#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "SharedLib.h"
#include "FileReaderUtils.h" // Include the shared utility

void ReadSongsFromFile(const std::string& aFilename, FSongRead aOnSongRead)
{
	if (aFilename.empty()) return;

	std::ifstream file(aFilename);
	if (!file.is_open())
	{
		// std::cerr << "Error: Could not open file " << aFilename << std::endl;
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

	// --- 2. Loop through the rest of the file (the data lines) ---
	while (keepReading && std::getline(file, line))
	{
		if (line.empty()) continue; // Skip empty lines

		std::istringstream lineStream(line);
		std::string artist, title, year, genre, source;

		// Parse the five semicolon-separated fields
		// Artist;Title;Year;Genre;Source
		if (std::getline(lineStream, artist, ';') &&
			std::getline(lineStream, title, ';') &&
			std::getline(lineStream, year, ';') &&
			std::getline(lineStream, genre, ';') &&
			std::getline(lineStream, source)) // Last one reads to end of line
		{
			if (aOnSongRead)
			{
				// Call the callback with all parameters
				if (!aOnSongRead(currentIndex, totalCount, artist, title, year, genre, source))
				{
					keepReading = false;
				}
				currentIndex++;
			}
		}
	}

	file.close();
}