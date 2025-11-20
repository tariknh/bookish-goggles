#pragma once
#include "ArrayWrapper.hpp"

template <typename T>
using FCompareFunction = bool(*)(const T, const T);

template <typename T>
class TQuickSort
{
private:
	/**
	 * @brief Swaps two items in the array.
	*/
	static void Swap(TArrayWrapper<T>& array, int i, int j)
	{
		T temp = array[i];
		array[i] = array[j];
		array[j] = temp;
	}
	/**
	 * @brief Partitions the array (Lomuto partition scheme).
	 */
	static int Partition(
		TArrayWrapper<T>& aArray,
		int aLow,
		int aHigh,
		FCompareFunction<T> aCompareFunc
	) {
		T pivot = (*aArray)[aHigh];
		int i = aLow; // Index of smaller element
		for (int j = aLow; j < aHigh; j++)
		{
			// Use the comparison function.
			// If current element is smaller than or equal to pivot
			if (aCompareFunc(aArray[j], pivot))
			{
				Swap(aArray, i, j);
				i++; // Increment index of smaller element
			}
		}
		Swap(aArray, i, aHigh);
		return i;
	}

	/**
 * @brief Recursive quick sort helper.
 */
	static void QuickSort(
		TArrayWrapper<T>* aArray,
		int aLow,
		int aHigh,
		FCompareFunction<T> aCompare
	) {
		if (aLow < aHigh)
		{
			// pi is partitioning index, arr[pi] is now at right place
			int pi = Partition(aArray, aLow, aHigh, aCompare);

			// Separately sort elements before and after partition
			QuickSort(aArray, aLow, pi - 1, aCompare);
			QuickSort(aArray, pi + 1, aHigh, aCompare);
		}
	}
public:
	/**
	 * @brief Public static Sort method for TArrayWrapper.
	 */
	static void Sort(TArrayWrapper<T>* aArray, FCompareFunction<T> aCompare) {
		if (aArray == nullptr || aArray->GetCount() < 2) {
			return; // No need to sort
		}
		QuickSort(aArray, 0, aArray->GetCount() - 1, aCompare);
	}
};