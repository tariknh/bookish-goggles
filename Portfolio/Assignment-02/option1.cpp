// Option 1 (Standard): Cruise Ship Manifest.

#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "option1.h"
#include "SharedLib.h"
#include "LinkedList.hpp"
#include "MergeSort.hpp"

namespace
{
constexpr int kEmployeeCount = 1500;

struct ManifestContext
{
	TLinkedList<TPerson> guests;
	TLinkedList<TPerson> employees;
	int totalProcessed{ 0 };
	std::mt19937 rng{ std::random_device{}() };
	std::uniform_int_distribution<int> cabinDist{ 1, 4 };
};

ManifestContext* gContext = nullptr;

std::string StatusToString(const TPersonStatus status)
{
	return (status == TPersonStatus::Employee) ? "Employee" : "Guest";
}

int CompareByName(const TPerson& lhs, const TPerson& rhs)
{
	int lastCmp = lhs.lastName.compare(rhs.lastName);
	if (lastCmp != 0)
	{
		return lastCmp;
	}
	return lhs.firstName.compare(rhs.firstName);
}

bool MergeCompareByName(const TPerson lhs, const TPerson rhs)
{
	return CompareByName(lhs, rhs) <= 0;
}

int CompareByCabin(const TPerson& lhs, const TPerson& rhs)
{
	if (lhs.cabinSize != rhs.cabinSize)
	{
		return (lhs.cabinSize < rhs.cabinSize) ? -1 : 1;
	}
	return CompareByName(lhs, rhs);
}

int PartitionByCabin(std::vector<TPerson>& people, int low, int high)
{
	TPerson pivot = people[high];
	int i = low;
	for (int j = low; j < high; ++j)
	{
		if (CompareByCabin(people[j], pivot) <= 0)
		{
			std::swap(people[i], people[j]);
			++i;
		}
	}
	std::swap(people[i], people[high]);
	return i;
}

void QuickSortByCabin(std::vector<TPerson>& people, int low, int high)
{
	if (low >= high)
	{
		return;
	}
	int pivotIndex = PartitionByCabin(people, low, high);
	QuickSortByCabin(people, low, pivotIndex - 1);
	QuickSortByCabin(people, pivotIndex + 1, high);
}

int BinarySearchGuest(const std::vector<TPerson>& guests, const std::string& first, const std::string& last)
{
	int low = 0;
	int high = static_cast<int>(guests.size()) - 1;
	while (low <= high)
	{
		int mid = low + (high - low) / 2;
		const TPerson& current = guests[mid];
		int cmp = current.lastName.compare(last);
		if (cmp == 0)
		{
			cmp = current.firstName.compare(first);
		}
		if (cmp == 0)
		{
			return mid;
		}
		if (cmp < 0)
		{
			low = mid + 1;
		}
		else
		{
			high = mid - 1;
		}
	}
	return -1;
}

std::vector<TPerson> CopyListToVector(TLinkedList<TPerson>& list)
{
	std::vector<TPerson> result;
	result.reserve(static_cast<std::size_t>(list.GetCount()));
	TLinkedListNode<TPerson>* node = list.GetHead()->GetNext();
	TLinkedListNode<TPerson>* tail = list.GetTail();
	while (node != tail)
	{
		result.push_back(node->GetData());
		node = node->GetNext();
	}
	return result;
}

void PrintSample(const std::vector<TPerson>& list, const std::string& title)
{
	std::cout << title << " (" << list.size() << ")" << std::endl;
	const std::size_t limit = std::min<std::size_t>(list.size(), 8);
	for (std::size_t i = 0; i < limit; ++i)
	{
		const TPerson& person = list[i];
		std::cout << "  " << person.lastName << ", " << person.firstName
			<< " - " << StatusToString(person.status)
			<< " - cabin " << person.cabinSize << std::endl;
	}
}

std::string ResolveDataFile()
{
	const char* candidates[] = {
		"DATA/random_names.txt",
		"../DATA/random_names.txt",
		"../../DATA/random_names.txt"
	};
	for (const char* path : candidates)
	{
		std::ifstream file(path);
		if (file.good())
		{
			return path;
		}
	}
	return {};
}

bool NameReadCallback(const int index, const int /*totalCount*/, const std::string& firstName, const std::string& lastName)
{
	if (!gContext)
	{
		return false;
	}
	TPersonStatus status = (index < kEmployeeCount) ? TPersonStatus::Employee : TPersonStatus::Guest;
	TPerson person(firstName, lastName, status, gContext->cabinDist(gContext->rng));
	if (status == TPersonStatus::Employee)
	{
		gContext->employees.Append(person);
	}
	else
	{
		gContext->guests.Append(person);
	}
	++gContext->totalProcessed;
	return true;
}
} // namespace

int RunApp()
{
	const std::string dataFile = ResolveDataFile();
	if (dataFile.empty())
	{
		std::cerr << "Could not locate random_names.txt" << std::endl;
		return 1;
	}

	ManifestContext context;
	gContext = &context;
	readNamesFromFile(dataFile, NameReadCallback);
	gContext = nullptr;

	std::cout << "Loaded " << context.totalProcessed << " people." << std::endl;
	std::cout << "Employees: " << context.employees.GetCount() << std::endl;
	std::cout << "Guests:    " << context.guests.GetCount() << std::endl;

	TMergeSort<TPerson>::Sort(&context.employees, MergeCompareByName);
	TMergeSort<TPerson>::Sort(&context.guests, MergeCompareByName);

	std::vector<TPerson> employees = CopyListToVector(context.employees);
	std::vector<TPerson> guests = CopyListToVector(context.guests);
	PrintSample(employees, "Employees (alphabetical)");
	PrintSample(guests, "Guests (alphabetical)");

	if (!guests.empty())
	{
		std::vector<TPerson> cabins = guests;
		QuickSortByCabin(cabins, 0, static_cast<int>(cabins.size()) - 1);
		PrintSample(cabins, "Guests (cabin grouping)");

		const TPerson& lookupTarget = guests[guests.size() / 2];
		int foundIndex = BinarySearchGuest(guests, lookupTarget.firstName, lookupTarget.lastName);
		std::cout << "Lookup sample: " << lookupTarget.firstName << " " << lookupTarget.lastName
			<< (foundIndex >= 0 ? " found." : " not found.") << std::endl;
	}
	else
	{
		std::cout << "Guest list is empty." << std::endl;
	}

	return 0;
}