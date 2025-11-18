#pragma once
#ifndef PRIORITY_QUEUE_HPP
#define PRIORITY_QUEUE_HPP

#include <stdexcept>

/**
 * @brief A generic Min-Heap implementation.
 * Stores items of type T, prioritized by a float value (lower is better).
 */
template <typename T>
class TPriorityQueue {
private:
	struct THeapNode {
		T data;
		float priority;
	};

	THeapNode* heapArray;
	int count;
	int capacity;

	/**
	 * @brief Swaps two nodes in the heap array.
	 */
	void Swap(int aIndexA, int aIndexB) {
		THeapNode temp = heapArray[aIndexA];
		heapArray[aIndexA] = heapArray[aIndexB];
		heapArray[aIndexB] = temp;
	}

	/**
	 * @brief Restores heap property by moving an item up.
	 */
	void HeapifyUp(int aIndex) {
		if (aIndex == 0) return;

		int parentIndex = (aIndex - 1) / 2;

		// If current node is smaller than parent (Min-Heap), swap
		if (heapArray[aIndex].priority < heapArray[parentIndex].priority) {
			Swap(aIndex, parentIndex);
			HeapifyUp(parentIndex);
		}
	}

	/**
	 * @brief Restores heap property by moving an item down.
	 */
	void HeapifyDown(int aIndex) {
		int leftChild = 2 * aIndex + 1;
		int rightChild = 2 * aIndex + 2;
		int smallest = aIndex;

		if (leftChild < count && heapArray[leftChild].priority < heapArray[smallest].priority) {
			smallest = leftChild;
		}

		if (rightChild < count && heapArray[rightChild].priority < heapArray[smallest].priority) {
			smallest = rightChild;
		}

		if (smallest != aIndex) {
			Swap(aIndex, smallest);
			HeapifyDown(smallest);
		}
	}

public:
	TPriorityQueue(int aCapacity) : count(0), capacity(aCapacity) {
		heapArray = new THeapNode[aCapacity];
	}

	~TPriorityQueue() {
		delete[] heapArray;
	}

	bool IsEmpty() const {
		return count == 0;
	}

	void Enqueue(T aData, float aPriority) {
		if (count >= capacity) {
			// In a real app, we would resize. Here we assume strict bounds.
			throw std::overflow_error("Priority Queue Overflow");
		}

		// Insert at end
		heapArray[count].data = aData;
		heapArray[count].priority = aPriority;

		// Bubble up
		HeapifyUp(count);
		count++;
	}

	T Dequeue() {
		if (IsEmpty()) {
			throw std::underflow_error("Priority Queue Underflow");
		}

		T result = heapArray[0].data;

		// Move last element to root
		heapArray[0] = heapArray[count - 1];
		count--;

		// Bubble down
		if (count > 0) {
			HeapifyDown(0);
		}

		return result;
	}
};

#endif // PRIORITY_QUEUE_HPP