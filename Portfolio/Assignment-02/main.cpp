// Mandatory-02.cpp : Defines the entry point for the application.
//

/*
Dear Student,

Remember to follow the coding standards and best practices discussed
in the portfolio assignment document.
Good luck with your portfolio!

NB: Do not delete the code below that prints the assignment and option info!

---------------------------------------------------------------------
*** HOW TO SWITCH BETWEEN OPTION 1 AND OPTION 2 ***
---------------------------------------------------------------------
You CANNOT switch options by changing this file.

1.  Go to the 'CMakeLists.txt' file for this assignment.
2.  Find the line:
		option(BUILD_ASSIGNMENT_OPTION_1 "..." ON)
3.  Change 'ON' (for Option 1) to 'OFF' (for Option 2).

*** VERY IMPORTANT: After changing the option ***
Your project will NOT update until you re-run the CMake configuration.

To force an update (e.g., in Visual Studio):
-   Right-click the 'CMakeLists.txt' file and select 'Configure Cache'.
-   OR, simply delete the 'out' / 'build' folder and rebuild the project.
---------------------------------------------------------------------
*/

#include <iostream>
#include <string_view>

static constexpr std::string_view AssignmentName = "Category 2: Sorting & Searching";

#if ASSIGNMENT_02_OPTION == 1
#include "option1.h"
static constexpr std::string_view AssignmentOption = "Option 1 (Standard): Cruise Ship Manifest.";
#elif ASSIGNMENT_02_OPTION == 2
#include "option2.h"
static constexpr std::string_view AssignmentOption = "Option 2 (Advanced): Combined Corporate Directory.";
#endif



int main(int argc, char* argv[])
{
	int appStatus = 0;
	std::cout << AssignmentName << std::endl;
	std::cout << AssignmentOption << std::endl;

	// Create only core or common code in main.cpp
	// Use the option header files to implement the specific assignment option logic
	appStatus = RunApp();
	return appStatus;
}
