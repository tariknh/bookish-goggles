#include <iostream>
#include <string>
#include <algorithm> 

#include "TExpressionTree.h"  
 #include "RedBlackTree.hpp"     
 #include "AVLTree.hpp"         

//string cleaner
std::string Trim(const std::string& str) {
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first) return str;
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

void PrintHelp() {
    std::cout << "\n=== COMMANDS ===\n";
    std::cout << "  set <var> = <val>   : Define a variable (e.g. 'set x = 10')\n";
    std::cout << "  calc <expression>   : Evaluate math (e.g. 'calc (x + 5) * 2')\n";
    std::cout << "  demo_avl            : Run the AVL Delete Extra Credit Demo\n";
    std::cout << "  help                : Show this menu\n";
    std::cout << "  vars                : Print variables\n";
    std::cout << "  exit                : Quit\n";
    std::cout << "------------------------------------------------------------\n";
}

void RunAVLDemo() {
    std::cout << "\n=== EXTRA CREDIT: AVL TREE DELETE DEMO ===\n";
    TAVLTree<int> avl;
    
    std::cout << "1. Inserting values: 10, 20, 30, 40, 50, 25\n";
    avl.Insert(10); avl.Insert(20); avl.Insert(30);
    avl.Insert(40); avl.Insert(50); avl.Insert(25);
    
    std::cout << "   Tree is balanced. Printing InOrder:\n   ";
    avl.PrintTree();

    std::cout << "2. Deleting 30 (Internal Node with 2 children)...\n";
    avl.Remove(30);
    
    std::cout << "   Deletion complete. Tree should rebalance.\n   ";
    avl.PrintTree();
    
    std::cout << "3. Deleting 40 (Leaf node)...\n";
    avl.Remove(40);
    std::cout << "   ";
    avl.PrintTree();
    std::cout << "=== END DEMO ===\n";
}

int RunApp() {
   
    TRedBlackTree<double> symbolTable; 
    TExpressionTree calculator;

    std::cout << "--- Assignment 3: Interpreted Calculator ---\n";
    std::cout << "---     (With RBT & AVL)    ---\n";
    
    // Here we just pre-make some constants :)
    symbolTable.Insert("pi", 3.14159265);
    symbolTable.Insert("e", 2.71828182);
    
    PrintHelp();

    std::string inputLine;
    while (true) {
        std::cout << ">> ";
        if (!std::getline(std::cin, inputLine)) break;
        if (inputLine == "exit") break;
        if (inputLine.empty()) continue;


        if (inputLine.substr(0, 4) == "help") {
            PrintHelp();
        }
        else if (inputLine == "demo_avl") {
            RunAVLDemo();
        }
        else if (inputLine.substr(0, 3) == "set") {
            // Format: "set x = 10"
            size_t eqPos = inputLine.find('=');
            if (eqPos != std::string::npos) {
                std::string varPart = inputLine.substr(3, eqPos - 3); // Extract " x "
                std::string valPart = inputLine.substr(eqPos + 1);    // Extract " 10"

                std::string var = Trim(varPart);
                try {
                    double val = std::stod(valPart);
                    symbolTable.Insert(var, val);
                    std::cout << "   Variable '" << var << "' set to " << val << std::endl;
                }
                catch (...) {
                    std::cout << "   Error: Invalid number format." << std::endl;
                }
            } else {
                std::cout << "   Usage: set <var> = <val>" << std::endl;
            }
        }
        else if (inputLine.substr(0, 4) == "calc") {
            std::string expr = inputLine.substr(4); // Skip "calc"
            
            if (calculator.BuildTree(expr)) {
                try {
                    double result = calculator.Evaluate(symbolTable);
                    std::cout << "   Result: " << result << std::endl;
                } catch (const std::exception& e) {
                    std::cout << "   Math Error: " << e.what() << std::endl;
                }
            } else {
                std::cout << "   Syntax Error: Check parentheses or operators." << std::endl;
            }
        }
		else if (inputLine.substr(0, 4) == "vars"){
			std::string expr = inputLine.substr(4); // Skip "vars"

			symbolTable.PrintInOrder();
		}
        else {
            std::cout << "   Unknown command. Type 'help' for list." << std::endl;
        }
    }

    return 0;
}