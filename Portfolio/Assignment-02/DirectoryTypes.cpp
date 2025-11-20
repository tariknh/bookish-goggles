#include "DirectoryTypes.h"
#include <random>

namespace
{
	static std::mt19937& GetPRNG()
	{
		static std::mt19937 engine{ std::random_device{}() };
		return engine;
	}
}

EDepartment GetRandomDepartment()
{
	std::uniform_int_distribution<int> dist(0, DepartmentCount - 1);
	int randomDept = dist(GetPRNG());
	return static_cast<EDepartment>(randomDept);
}

std::string DepartmentToString(EDepartment dept)
{
	switch (dept)
	{
	case DIT: return "Inforation Tech";
	case Finance: return "Finance";
	case HR: return "HR";
	case Marketing: return "Marketing";
	case Sales: return "Sales";
	default: return "Unknown Department";
	}
}