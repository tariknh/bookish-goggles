#include "option2.h"
#include <string>
#include "LinkedList.hpp"
#include "DirectoryTypes.h"
#include <iostream>
#include <SharedLib.h>
#include <MergeSort.hpp>

// Use std namespace
using namespace std;

static TLinkedList<TEmployee*>* employeeList = nullptr;

/**
 * @brief Helper function to print a header.
 */
static void PrintHeader(const std::string& aTitle)
{
	std::cout << "\n--- " << aTitle << " ---" << std::endl;
}


/**
 * @brief Helper function to print the first 10 employees from the list.
 */
static void PrintListHead(TLinkedList<TEmployee*>* aList, const std::string& aTitle)
{
	PrintHeader(aTitle);
	TLinkedListNode<TEmployee*>* current = aList->GetHead()->GetNext();
	int i = 0;
	while (i < 10 && current != aList->GetTail())
	{
		TEmployee* emp = current->GetData();
		std::cout << "  " << emp->lastName << ", "
			<< emp->firstName << std::endl;
		current = current->GetNext();
		i++;
	}
}

static bool RandomNamesOnRead(
	const int aIndex,
	const int aTotal,
	const string& aFirstName,
	const string& aLastName
) {
	EDepartment dept = GetRandomDepartment();
	TEmployee* newEmployee = new TEmployee(aIndex, aFirstName, aLastName, dept);
	employeeList->Append(newEmployee);

	return true;
}

static bool CompareByLastName(TEmployee* aEmp1, TEmployee* aEmp2) {
	if (aEmp1->lastName != aEmp2->lastName) {
		return aEmp1->lastName < aEmp2->lastName;
	}
	return aEmp1->firstName < aEmp2->firstName;
}


int RunApp() {
	string fileName = "random_names.txt";
	employeeList = new TLinkedList<TEmployee*>(true); // List owns the data
	readNamesFromFile(fileName, RandomNamesOnRead);

	cout << "Loaded " << employeeList->GetCount() << " employees from file." << std::endl;

	// --- TASK 1: MERGE SORT (MASTER LIST) ---
	std::cout << "\r\n--- TASK 1: MERGE SORT (MASTER LIST) ---" << std::endl;
	TMergeSort<TEmployee*>::Sort(
		employeeList,
		CompareByLastName
	);

	PrintListHead(employeeList, "First 10 Employees After Merge Sort (Master List)");


	std::cout << "\r\nAll tasks complete." << std::endl;
	delete employeeList;
	return 0;
}