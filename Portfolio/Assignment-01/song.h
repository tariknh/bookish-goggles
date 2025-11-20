#pragma once
#include <string>
#include <iostream>
struct TSong
{
	int songID;
	std::string title;
	std::string artist;
	std::string year;
	std::string genre;
	std::string source;

	TSong(
		int aIndex,
		std::string aArtist,
		std::string aTitle,
		std::string aYear,
		std::string aGenre,
		std::string aSource)
		: songID(aIndex),
		artist(aArtist),
		title(aTitle),
		year(aYear),
		genre(aGenre),
		source(aSource) {
	}

	void printOut() const
	{
		std::cout << "Song ID: " << songID << ", ";
		std::cout << "Artist: " << artist << ", ";
		std::cout << "Title: " << title << ", ";
		std::cout << "Year: " << year << ", ";
		std::cout << "Genre: " << genre << ", ";
		std::cout << "Source: " << source << "\r\n";
	}

}; // End of TSong