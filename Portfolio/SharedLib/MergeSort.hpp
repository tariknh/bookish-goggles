#pragma once
#include "LinkedList.hpp"

template <typename T>
using FCompareFunction = bool(*)(const T, const T);

template <typename T>
class TMergeSort
{
private:
	// static merge
	static TLinkedListNode<T>* Merge(
		TLinkedListNode<T>* aNodeA,
		TLinkedListNode<T>* aNodeB,
		FCompareFunction<T> aCompareFunc
	) {
		TLinkedListNode<T> tempHead{ T() };
		TLinkedListNode<T>* current{ &tempHead };
		while (aNodeA != nullptr && aNodeB != nullptr)
		{
			if (aCompareFunc(aNodeA->data, aNodeB->data))
			{
				current->next = aNodeA;
				aNodeA->prev = current;
				aNodeA = aNodeA->next;
			}
			else {
				current->next = aNodeB;
				aNodeB->prev = current;
				aNodeB = aNodeB->next;
			}
			current = current->next;
		} // end of while

		// Append the remainder of whichever list is not empty
		if (aNodeA != nullptr)
		{
			current->next = aNodeA;
			aNodeA->prev = current;
		}
		else if (aNodeB != nullptr)
		{
			current->next = aNodeB;
			aNodeB->prev = current;
		}
		// The sorted list starts *after* the dummy head
		TLinkedListNode<T>* sortedHead = tempHead.next;
		if (sortedHead) {
			sortedHead->prev = nullptr; // Set the prev of the new head to nullptr
		}
		return sortedHead;
	}// end of Merge


	/**
	 * @brief Recursive merge sort helper.
	 */
	static TLinkedListNode<T>* MergeSort(
		TLinkedListNode<T>* aHead,
		int aCount,
		FCompareFunction<T> aCompareFunc
	) {
		// Base case: 0 or 1 element
		if (aCount <= 1) {
			return aHead;
		}
		// 1. Find the middle
		int mid = aCount / 2;
		TLinkedListNode<T>* listA = aHead;
		TLinkedListNode<T>* listB = aHead;
		for (int i = 0; i < mid; ++i) {
			listB = listB->next;
		}
		// 2. Split the list into two halves
		if (listB->prev != nullptr) {
			listB->prev->next = nullptr; // Terminate first half
			listB->prev = nullptr;       // Start second half
		}

		// 3. Recursively sort both halves
		listA = MergeSort(listA, mid, aCompareFunc);
		listB = MergeSort(listB, aCount - mid, aCompareFunc);

		// 4. Merge the sorted halves
		return Merge(listA, listB, aCompareFunc);
	}

public:
	/**
 * @brief Public static Sort method for TLinkedList.
 */
	static void Sort(TLinkedList<T>* aList, FCompareFunction<T> aCompare)
	{
		if (aList == nullptr || aList->IsEmpty() || aList->GetCount() < 2) {
			return; // Nothing to sort
		}
		// Detach the list from its dummy head and tail
		TLinkedListNode<T>* originalHeadNode = aList->GetHead()->next;
		TLinkedListNode<T>* originalTailNode = aList->GetTail()->prev;
		originalHeadNode->prev = nullptr;
		originalTailNode->next = nullptr;

		// Perform merge sort
		int count = aList->GetCount();
		TLinkedListNode<T>* sortedHead = MergeSort(originalHeadNode, count, aCompare);

		//Find new tail
		TLinkedListNode<T>* newTail = sortedHead;
		while (newTail->next != nullptr) {
			newTail = newTail->next;
		}

		// Reattach to dummy head and tail
		TLinkedListNode<T>* dummyHead = aList->GetHead();
		TLinkedListNode<T>* dummyTail = aList->GetTail();
		dummyHead->next = sortedHead;
		sortedHead->prev = dummyHead;

		dummyTail->prev = newTail;
		newTail->next = dummyTail;
	}
};