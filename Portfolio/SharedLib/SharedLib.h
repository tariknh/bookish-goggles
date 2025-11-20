#pragma once
#ifndef SHARED_LIB_H
#define SHARED_LIB_H
#include <string>
#include <functional>

/// <summary>
/// Delegate type for processing a name read from a file.
/// </summary>
/// <param name="aIndex">The index of the name (0-based).</param>
/// <param name="aTotalCount">The total number of names.</param>
/// <param name="aFirstName">The first name read from the file.</param>
/// <param name="aLastName">The last name read from the file.</param>
/// <returns>Returns true to continue reading, false to stop.</returns>
typedef bool (*FNameRead)(
	const int aIndex,
	const int aTotalCount,
	const std::string& aFirstName,
	const std::string& aLastName
	);

/// <summary>
/// Use this function to read names from a specified file and process them using a callback function.
/// </summary>
///	<function>readNamesFromFile</function>
///	<description>Reads names from a specified file and invokes a callback for each name read.</description>
/// <param name="aFilename">The path to the file containing names.</param>
/// <param name="aOnNameRead">A callback function that is called for each name read. It takes two parameters: firstName and lastName. If the callback returns false, the reading process stops.</param>
/// <param name="firstName">The first name read from the file.</param>
/// <param name="lastName">The last name read from the file.</param>
/// <returns>None.</returns>
void readNamesFromFile(const std::string& aFilename, FNameRead aOnNameRead);

/// <summary>
/// Delegate type for processing a node read from the file.
/// </summary>
/// <description>Function pointer type for a callback that processes nodes read from a file.</description>
/// <param name="aIndex">The index of the node (0-based).</param>
/// <param name="aTotalCount">The total number of nodes.</param>
/// <param name="aNode">The node std::string.</param>
/// <returns>Returns true to continue reading, false to stop.</returns>
typedef bool (*FNodeRead)(const int aIndex, const int aTotalCount, const std::string& aNode);

/// <summary>
/// Delegate type for processing an edge read from the file.
/// </summary>
/// <description>Function pointer type for a callback that processes edges read from a file.</description>
/// <param name="aIndex">The index of the edge (0-based).</param>
/// <param name="aTotalCount">The total number of edges.</param>
/// <param name="aFromNode">The from node std::string.</param>
/// <param name="aToNode">The to node std::string.</param>
/// <param name="aWeight">The weight of the edge.</param>
/// <returns>Returns true to continue reading, false to stop.</returns>
typedef bool (*FEdgeRead)(const int aIndex, const int aTotalCount, const std::string& aFromNode, const std::string& aToNode, float aWeight);

/// </summary>
/// Use this function to read a graph from a specified file and process its nodes and edges using callback functions.
/// </summary>
/// <function>readGraphFromFile</function>
/// <description>
/// Reads a graph from a specified file and invokes callbacks for each node and edge read.
/// All nodes are read first, followed by edges.
/// </description>
/// <param name="aFilename">The path to the file containing the graph data.</param>
/// <param name="aOnNodeRead">A callback function that is called for each node read. It takes one parameter: the node std::string. If the callback returns false, the reading process stops.</param>
/// <param name="aOnEdgeRead">A callback function that is called for each edge read. It takes three parameters: the fromNode std::string, the toNode std::string, and the weight float. If the callback returns false, the reading process stops.</param>
void readGraphFromFile(const std::string& aFilename, FNodeRead aOnNodeRead, FEdgeRead aOnEdgeRead);


/// <summary>
/// Delegate type for processing a song read from the file.
/// </summary>
/// <param name="aIndex">The index of the song (0-based).</param>
/// <param name="aTotalCount">The total number of songs.</param>
/// <param name="aArtist">The artist.</param>
/// <param name="aTitle">The title.</param>
/// <param name="aYear">The release year (as a std::string).</param>
/// <param name="aGenre">The genre.</param>
/// <param name="aSource">The source.</param>
/// <returns>Returns true to continue reading, false to stop.</returns>
typedef bool (*FSongRead)(
	const int aIndex,
	const int aTotalCount,
	const std::string& aArtist,
	const std::string& aTitle,
	const std::string& aYear,
	const std::string& aGenre,
	const std::string& aSource
	);

/// <summary>
/// Reads song data from a file and processes them using a callback.
/// This function automatically skips the "records:=" header.
/// </summary>
/// <param name="aFilename">The path to the file (e.g., "songs.txt").</param>
/// <param name="aOnSongRead">The callback function called for each song.</param>
void ReadSongsFromFile(const std::string& aFilename, FSongRead aOnSongRead);


#endif // SHARED_LIB_H