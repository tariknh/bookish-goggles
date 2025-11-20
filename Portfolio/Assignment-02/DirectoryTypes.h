#include <string>

enum EDepartment
{
	DIT,
	Finance,
	HR,
	Marketing,
	Sales,
	DepartmentCount // Helper to get the count of departments, for easear random
};

struct TEmployee
{
	int ID;
	std::string firstName;
	std::string lastName;
	EDepartment department;

	TEmployee(int id, const std::string& fName, const std::string& lName, EDepartment dept)
		: ID(id), firstName(fName), lastName(lName), department(dept) {
	}
};

EDepartment GetRandomDepartment();
std::string DepartmentToString(EDepartment dept);