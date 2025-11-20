// option1.h : Option 1 (Standard): Cruise Ship Manifest.

#pragma once

#ifndef OPTION1_H
#define OPTION1_H

#include <string>
#include <utility>

enum class TPersonStatus
{
	Guest,
	Employee
};

struct TPerson
{
	std::string firstName;
	std::string lastName;
	TPersonStatus status{ TPersonStatus::Guest };
	int cabinSize{ 0 };

	TPerson() = default;
	TPerson(std::string first, std::string last, TPersonStatus personStatus, int cabin)
		: firstName(std::move(first))
		, lastName(std::move(last))
		, status(personStatus)
		, cabinSize(cabin)
	{
	}
};

int RunApp();


#endif // OPTION1_H
