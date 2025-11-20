#pragma once

#include <stdexcept>
#include <type_traits>

template <typename T>
class TArrayWrapper
{
private:
	T* items;           // Pointer to the data on the heap
	int count;          // How many items are currently in the array
	int size;           // The total size (capacity) of the array
	bool isDataOwner;  // Flag for memory management
public:
	/**
	 * @brief Constructor for a fixed-size array.
	 * @param aSize The exact number of items this array will hold.
	 * @param aIsDataOwner True if this array owns and must delete the T* data.
	 */
	TArrayWrapper(int aSize, bool aIsDataOwner = false)
		: items(nullptr), count(0), size(aSize), isDataOwner(aIsDataOwner)
	{
		if (aIsOwner && !std::is_pointer<T>::value) {
			throw std::invalid_argument("TArrayWrapper: aIsDataOwner can only be true for pointer types.");
		}
		if (aSize <= 0) {
			throw std::invalid_argument("TArrayWrapper: Size must be greater than zero.");
		}
		// Allocate memory for the array ONCE!
		items = new T[aSize];
	}
	/**
		 * @brief Destructor.
		 */
	~TArrayWrapper() {
		//TODO: Implement destructor to free memory if isDataOwner is true

		// Always delete the array container itself
		delete[] items;
	}

	void Add(const T aItem) {
		if (count >= size) {
			throw std::out_of_range("TArrayWrapper: Cannot add more items, array is full.");
		}
		items[count++] = aItem;
	}

	// ... (operator[] and GetCount() remain the same) ...
	const T& operator[](int index) {
		if (index < 0 || index >= count) {
			throw std::out_of_range("TArrayWrapper: Index out of range.");
		}
		return items[index];
	}

	T& operator[](int aIndex) const
	{
		if (aIndex < 0 || aIndex >= count)
		{
			throw std::out_of_range("TArrayWrapper: Index out of range.");
		}
		return items[aIndex];
	}

	int GetCount() const {
		return count;
	}
};