#pragma once
#include "song.h"

class TSongListNode {
public:
	TSong* song;
	TSongListNode* prev;
	TSongListNode* next;

	TSongListNode(TSong* aSong)
		: song(aSong), prev(nullptr), next(nullptr) {
	}
};

class TSongList {
private:
	TSongListNode* head;
	TSongListNode* tail;
	bool isDataOwner; // Responsible for deleting data

	void Clear();
public:
	TSongList(bool aIsDataOwner);
	~TSongList();
	bool IsEmpty() const;

	void Append(TSong* aSong);
	void Prepend(TSong* aSong);
	TSong* GetFirstSong() const;
	TSong* GetNextSong(TSong* aCurrentSong) const;
	TSong* GetPreviousSong(TSong* aCurrentSong) const;

	friend class TSongQueue;
	friend class TSongStack;

};

class TSongQueue : public TSongList {
public:
	TSongQueue();

	void Enqueue(TSong* aSong);
	// Add priority enqueue, to the front of the queue
	void EnqueuePriority(TSong* aSong);

	TSong* Dequeue();
};

class TSongStack : public TSongList {
public:
	TSongStack();

	void Push(TSong* aSong);

	TSong* Pop();
};