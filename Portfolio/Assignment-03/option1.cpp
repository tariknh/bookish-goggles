#include "option2.h"
#include "TExpressionTree.h"     // The Calculator logic
#include "BinarySearchTable.hpp" // The Symbol Table
#include <iostream>
#include <string>

void PrintHelp() {
	std::cout << "Commands:\n";
	std::cout << "  set <var> = <val>   : Define a variable (e.g. 'set x = 10')\n";
	std::cout << "  calc <expression>   : Evaluate math (e.g. 'calc (x + 5) * 2')\n";
	std::cout << "  vars                : List variables (Implementation Optional)\n";
	std::cout << "  help                : Show this menu\n";
	std::cout << "  exit                : Quit\n";
	std::cout << "------------------------------------------------------------\n";
}

int RunApp() {
	// 1. Instantiate Core Systems
	TBinarySearchTable<double> symbolTable;
	TExpressionTree calculator;

	std::cout << "--- Interpreted Calculator (Tree-Based) ---\n";
	PrintHelp();

	std::string inputLine;
	while (true) {
		std::cout << ">> ";
		if (!std::getline(std::cin, inputLine) || inputLine == "exit") {
			break;
		}
		if (inputLine.empty()) continue;

		// Basic Command Parsing
		if (inputLine.substr(0, 4) == "help") {
			PrintHelp();
		}
		else if (inputLine.substr(0, 3) == "set") {
			// Expected format: "set x = 10.5"
			// Simple parsing logic for the command line interface
			size_t eqPos = inputLine.find('=');
			if (eqPos != std::string::npos) {
				// Extract name: "set " is 4 chars. 
				// We need to trim spaces, but for simplicity assuming "set x = ..."
				std::string varPart = inputLine.substr(4, eqPos - 4);
				std::string valPart = inputLine.substr(eqPos + 1);

				// Trim simple spaces
				while (!varPart.empty() && isspace(varPart.back())) varPart.pop_back();
				while (!varPart.empty() && isspace(varPart.front())) varPart.erase(0, 1);

				try {
					double val = std::stod(valPart);
					symbolTable.Insert(varPart, val);
					std::cout << "Variable '" << varPart << "' set to " << val << std::endl;
				}
				catch (...) {
					std::cout << "Error: Invalid number format." << std::endl;
				}
			}
			else {
				std::cout << "Usage: set <variable> = <value>" << std::endl;
			}
		}
		else if (inputLine.substr(0, 4) == "calc") {
			// Extract expression
			std::string expr = inputLine.substr(4);

			// 1. Build the Tree
			if (calculator.BuildTree(expr)) {
				// 2. Evaluate using the Symbol Table
				double result = calculator.Evaluate(symbolTable);
				std::cout << "Result: " << result << std::endl;
			}
		}
		else {
			std::cout << "Unknown command. Try 'calc 5 + 5' or 'set x = 10'." << std::endl;
		}
	}

	return 0;
}