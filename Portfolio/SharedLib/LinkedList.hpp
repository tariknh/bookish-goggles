#pragma once
#include <iostream>
#include <type_traits>
/**
	* @brief TLinkedListNode class represents a node in a linked list.
*/
//Define TLinkedList class
template <typename T>
class TLinkedList;

template <typename T>
class TMergeSort;


template <typename T>
class TLinkedListNode
{
private:
	T data;
	TLinkedListNode* prev;
	TLinkedListNode* next;
public:
	TLinkedListNode(const T& data)
		: data(data), prev(nullptr), next(nullptr) {
	}

	void SwapNextPrev()
	{
		TLinkedListNode* temp = next;
		next = prev;
		prev = temp;
	}

	T GetData() const {
		return data;
	}

	TLinkedListNode* GetNext() const {
		return next;
	}

	friend class TLinkedList<T>;
	friend class TMergeSort<T>;
};

/*
	*@brief TLinkedList class represents a doubly linked list.
*/
template <typename T>
class TLinkedList
{
private:
	// We need big O(1) for adding/removing from head/tail
	TLinkedListNode<T>* head;
	TLinkedListNode<T>* tail;
	int count;
	bool isDataOwner; // Indicates if the list owns the data and should delete it

	void Clear() {
		int deletedNodes = 0;
		int deletedData = 0;
		TLinkedListNode<T>* current = head->next;
		while (current != tail) {
			TLinkedListNode<T>* nodeToDelete = current;
			current = current->next;
			if (isDataOwner) {
				if constexpr (std::is_pointer_v<T>) {
					delete nodeToDelete->data;
					deletedData++;
				}
				else {
					std::cerr << "Warning: TLinkedList attempted to delete non-pointer data. Ignoring." << std::endl;
				}
			}
			delete nodeToDelete;
			deletedNodes++;
		}
		head->next = tail;
		tail->prev = head;
		count = 0;
		// Console logging for debug purposes
		std::cout << "Cleared list:" << std::endl;
		std::cout << "Deleted " << deletedNodes << " nodes" << std::endl;
		std::cout << "Deleted " << deletedData << " data items." << std::endl;
	}

public:
	TLinkedList(bool ownsData = false)
		: head(nullptr), tail(nullptr), count(0), isDataOwner(ownsData) {
		//  We are using dummy head and tail nodes to simplify add/remove logic
		head = new TLinkedListNode<T>(T());
		tail = new TLinkedListNode<T>(T());
		head->next = tail;
		tail->prev = head;
	}

	~TLinkedList() {
		Clear();
		delete head;
		delete tail;
	}

	bool IsEmpty() const {
		return count == 0;
	}

	int GetCount() const { return count; }

	TLinkedListNode<T>* GetHead() const { return head; }
	TLinkedListNode<T>* GetTail() const { return tail; }


	void Append(const T& aData) {
		TLinkedListNode<T>* newNode = new TLinkedListNode<T>(aData);
		newNode->prev = tail->prev;
		newNode->next = tail;
		tail->prev->next = newNode;
		tail->prev = newNode;
		count++;
	}

	void Prepend(const T& aData) {
		TLinkedListNode<T>* newNode = new TLinkedListNode<T>(aData);
		newNode->next = head->next;
		newNode->prev = head;
		head->next->prev = newNode;
		head->next = newNode;
		count++;
	}

	T RemoveHead() {
		if (IsEmpty()) throw std::runtime_error("List is empty");
		TLinkedListNode<T>* nodeToRemove = head->next;
		T data = nodeToRemove->data;
		// Re-link the list
		TLinkedListNode<T>* nextNode = nodeToRemove->next;
		head->next = nextNode;
		nextNode->prev = head;
		delete nodeToRemove;
		count--;
		// Users are responsible for deleting data if isDataOwner is true
		return data;
	}

	T RemoveTail() {
		if (IsEmpty()) throw std::runtime_error("List is empty");
		TLinkedListNode<T>* nodeToRemove = tail->prev;
		T data = nodeToRemove->data;
		// Re-link the list
		TLinkedListNode<T>* prevNode = nodeToRemove->prev;
		tail->prev = prevNode;
		prevNode->next = tail;
		delete nodeToRemove;
		count--;
		// Users are responsible for deleting data if isDataOwner is true
		return data;
	}

};
