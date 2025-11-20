// Option 2 (Advanced): Console Music Player.

#include <iostream>
#include "option2.h"
#include "SharedLib.h"
#include "song.h"
#include "songList.h"

static TSongList* songs = nullptr;
static TSongQueue* playQueue = nullptr;
static TSongQueue* wishQueue = nullptr;
static TSongStack* songHistory = nullptr;

static bool SongReadCallback(const int aIndex, const int aTotalCount, const std::string& aArtist, const std::string& aTitle, const std::string& aYear, const std::string& aGenre, const std::string& aSource) {
	TSong* song = new TSong(aIndex, aArtist, aTitle, aYear, aGenre, aSource);
	//song->printOut();
	songs->Append(song);
	return true;
}


int RunApp() {
	// Implement the Console Music Player application logic here

	std::string filename = "F:\\IKT203\\VisualStudio\\DATA\\songs.txt"; // Specify the path to the songs file

	songs = new TSongList(true); // Create a song list that owns its data

	ReadSongsFromFile(filename, SongReadCallback);

	// Example: Print out all songs. forward and backward
	TSong* currentSong = songs->GetFirstSong();
	std::cout << "Songs in forward order:\r\n";
	while (1) {
		currentSong->printOut();
		TSong* temp = songs->GetNextSong(currentSong);
		if (temp == nullptr) break;
		currentSong = temp;
	}
	std::cout << "Songs in backward order:\r\n";
	// currentSong is now at the last song
	while (1) {
		currentSong->printOut();
		TSong* temp = songs->GetPreviousSong(currentSong);
		if (temp == nullptr) break;
		currentSong = temp;
	}

	std::cout << "\r\nCreating Play Queue, with 10 songs from the song list.\r\n";
	playQueue = new TSongQueue(); // Create a play queue that does not own its data
	currentSong = songs->GetFirstSong();
	for (int i = 0; i < 10; i++) {
		currentSong->printOut();
		playQueue->Append(currentSong);
		currentSong = songs->GetNextSong(currentSong);
	}
	// currentSong is now at the 11th song
	std::cout << "\r\nCreating Wish Queue with 5 songs from the song list.\r\n";
	wishQueue = new TSongQueue(); // Create a wish queue that does not own its data
	for (int i = 0; i < 5; i++) {
		currentSong->printOut();
		wishQueue->Append(currentSong);
		currentSong = songs->GetNextSong(currentSong);
	}

	songHistory = new TSongStack(); // Create a song history stack that does not own its data

	int songsPlayed = 0;
	// Play all songs in the play queue, and remove them from the play queue
	std::cout << "\r\nPlaying songs from the Play Queue:\r\n";
	while (playQueue->IsEmpty() == false) {
		songsPlayed++;
		if (wishQueue->IsEmpty() == false) {
			// Move the first song from the wish queue to the play queue
			TSong* wishSong = wishQueue->GetFirstSong();
			std::cout << "\r\nPlaying from Wish Queue:\r\n";
			wishSong->printOut();
			wishQueue->Dequeue();
			songHistory->Push(wishSong);
		}
		else {
			TSong* songToPlay = playQueue->GetFirstSong();
			std::cout << "\r\nNow playing from Play Queue:\r\n";
			songToPlay->printOut();
			// Remove the song from the play queue
			playQueue->Dequeue();
			songHistory->Push(songToPlay);
		}
		if (songsPlayed == 4) {
			std::cout << "\r\n4 songs played, going back to 3rd last played song from history:\r\n";
			TSong* lastPlayed = nullptr;
			for (int i = 0; i < 3; i++) {
				lastPlayed = songHistory->Pop();
			}
			std::cout << "Add back to Wish Queue, as the first song to be played:\r\n";
			lastPlayed->printOut();
			wishQueue->EnqueuePriority(lastPlayed); // Add back to wish queue, at the end, but it will be played first
		}
		else if (songsPlayed == 14) {
			std::cout << "\r\n14 songs played, add 5 more songs to Wish Queue from song list:\r\n";
			// currentSong is now at the 16th song
			for (int i = 0; i < 5; i++) {
				wishQueue->Append(currentSong);
				currentSong = songs->GetNextSong(currentSong);
			}
		}
	}


	// Clean up
	delete playQueue;
	delete wishQueue;
	delete songHistory;
	delete songs; // Clean up the song list and its data
	return 0;
}