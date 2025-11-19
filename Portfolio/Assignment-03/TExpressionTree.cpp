#include "TExpressionTree.h"
#include <cctype>   // isdigit, isalpha, isspace
#include <cmath>    // pow, sin, cos, log, etc.
#include <stdexcept> // runtime_error
#include "RedBlackTree.hpp"

// =========================================================
// Constructor / Destructor
// =========================================================

TExpressionTree::TExpressionTree() 
    : root(nullptr), expression(nullptr), pos(0) {
}

TExpressionTree::~TExpressionTree() {
    DeleteSubtree(root);
}

void TExpressionTree::DeleteSubtree(TExpressionNode* aNode) {
    // Post-order deletion: Left, Right, then Root
    if (!aNode) return;

    DeleteSubtree(aNode->left);
    DeleteSubtree(aNode->right);

    delete aNode;
}

// =========================================================
// Public Interface
// =========================================================

bool TExpressionTree::BuildTree(const std::string& aExpression) {
    // 1. Cleanup old tree
    DeleteSubtree(root);
    root = nullptr;

    // 2. Reset parser state
    expression = &aExpression;
    pos = 0;

    // 3. Start parsing
    try {
        root = ParseExpression();

        // Check if we consumed the whole string (ignoring trailing spaces)
        SkipWhitespace();
        if (Peek() != '\0') {
            // Found unexpected characters at the end
            DeleteSubtree(root);
            root = nullptr;
            return false;
        }
        return true;

    } catch (...) {
        // If any parsing error occurs, clean up and fail
        DeleteSubtree(root);
        root = nullptr;
        return false;
    }
}

double TExpressionTree::Evaluate(const TRedBlackTree<double>& aSymTable) const {
    if (!root) {
        throw std::runtime_error("Cannot evaluate: Tree is empty.");
    }
    return EvaluateNode(root, aSymTable);
}

// =========================================================
// Evaluation Logic (Recursive)
// =========================================================

double TExpressionTree::EvaluateNode(TExpressionNode* aNode, const TRedBlackTree<double>& aSymTable) const {
    if (!aNode) return 0.0;

    // CASE 1: Numbers
    if (aNode->type == ENodeType::OPERAND) {
        return aNode->value;
    }

    // CASE 2: Variables
    if (aNode->type == ENodeType::VARIABLE) {
        // Look up in the template symbol table
        // Assuming your BST has a Search method that throws or returns a value
        // Adjust 'Search' to whatever your BST method is named (e.g., get, find)
        try {
            return aSymTable.Search(aNode->name); 
        } catch (...) {
            throw std::runtime_error("Undefined variable: " + aNode->name);
        }
    }

    // CASE 3: Operators
    if (aNode->type == ENodeType::OPERATOR) {
        double leftVal = EvaluateNode(aNode->left, aSymTable);
        double rightVal = EvaluateNode(aNode->right, aSymTable);

        switch (aNode->op) {
            case '+': return leftVal + rightVal;
            case '-': return leftVal - rightVal;
            case '*': return leftVal * rightVal;
            case '^': return std::pow(leftVal, rightVal);
            case '/': 
                if (rightVal == 0) throw std::runtime_error("Division by zero");
                return leftVal / rightVal;
            default:  throw std::runtime_error("Unknown operator");
        }
    }

    // CASE 4: Functions
    if (aNode->type == ENodeType::FUNCTION) {
        // Functions usually have the argument in the LEFT child
        double arg = EvaluateNode(aNode->left, aSymTable);
        
        if (aNode->name == "sin") return std::sin(arg);
        if (aNode->name == "cos") return std::cos(arg);
        if (aNode->name == "tan") return std::tan(arg);
        if (aNode->name == "log") return std::log10(arg);
        if (aNode->name == "ln")  return std::log(arg);
        if (aNode->name == "exp") return std::exp(arg);
        if (aNode->name == "sqrt") {
             if (arg < 0) throw std::runtime_error("Sqrt of negative number");
             return std::sqrt(arg);
        }
        
        throw std::runtime_error("Unknown function: " + aNode->name);
    }

    return 0.0;
}

// =========================================================
// Parser Helpers
// =========================================================

char TExpressionTree::Peek() {
    if (pos >= expression->length()) return '\0';
    return (*expression)[pos];
}

char TExpressionTree::Get() {
    char c = Peek();
    if (c != '\0') pos++;
    return c;
}

bool TExpressionTree::Match(char aExpected) {
    SkipWhitespace();
    if (Peek() == aExpected) {
        Get();
        return true;
    }
    return false;
}

void TExpressionTree::SkipWhitespace() {
    while (pos < expression->length() && std::isspace((*expression)[pos])) {
        pos++;
    }
}

// =========================================================
// Recursive Descent Grammar Implementation
// =========================================================

// Grammar: Expression -> Term { (+|-) Term }
TExpressionNode* TExpressionTree::ParseExpression() {
    TExpressionNode* left = ParseTerm();

    while (true) {
        SkipWhitespace();
        char c = Peek();
        if (c == '+' || c == '-') {
            Get(); // Consume operator
            TExpressionNode* right = ParseTerm();
            
            // Create a new parent node for this operation
            TExpressionNode* parent = new TExpressionNode(c, left, right);
            left = parent; // The new parent becomes the left child for the next op
        } else {
            break;
        }
    }
    return left;
}

// Grammar: Term -> Factor { (*|/|^) Factor }
TExpressionNode* TExpressionTree::ParseTerm() {
    TExpressionNode* left = ParseFactor();

    while (true) {
        SkipWhitespace();
        char c = Peek();
        if (c == '*' || c == '/' || c == '^') {
            Get(); // Consume operator
            TExpressionNode* right = ParseFactor();

            // Create parent node
            TExpressionNode* parent = new TExpressionNode(c, left, right);
            left = parent;
        } else {
            break;
        }
    }
    return left;
}

// Grammar: Factor -> Number | (Expression) | Variable | Function
TExpressionNode* TExpressionTree::ParseFactor() {
    SkipWhitespace();
    char c = Peek();

    // 1. Parentheses
    if (Match('(')) {
        TExpressionNode* node = ParseExpression();
        if (!Match(')')) {
            // Clean up if parenthesis mismatch
            DeleteSubtree(node);
            throw std::runtime_error("Missing closing parenthesis");
        }
        return node;
    }

    // 2. Numbers
    if (isdigit(c) || c == '.') {
        double val = ParseNumber();
        return new TExpressionNode(val);
    }

    // 3. Variables or Functions
    if (isalpha(c)) {
        std::string name = ParseName();
        
        // Check if it is a function call, e.g., sin(...)
        SkipWhitespace();
        if (Peek() == '(') {
            // It's a function!
            Match('('); // Consume '('
            TExpressionNode* arg = ParseExpression(); // Argument is the expression inside
            if (!Match(')')) {
                DeleteSubtree(arg);
                throw std::runtime_error("Missing closing parenthesis for function");
            }
            
            // Create Function Node: Name=funcName, Type=FUNCTION, Left=Argument
            TExpressionNode* funcNode = new TExpressionNode(name, ENodeType::FUNCTION);
            funcNode->left = arg; 
            return funcNode;
        } 
        else {
            // It's just a variable
            return new TExpressionNode(name, ENodeType::VARIABLE);
        }
    }

    // 4. Handling negative numbers (Unary minus)
    if (c == '-') {
        Get(); // Consume minus
        TExpressionNode* right = ParseFactor();
        // Represent -X as (0 - X)
        return new TExpressionNode('-', new TExpressionNode(0.0), right);
    }

    throw std::runtime_error("Unexpected character in expression");
}

double TExpressionTree::ParseNumber() {
    SkipWhitespace();
    size_t startPos = pos;
    
    // Consume digits and optional dot
    bool dotFound = false;
    while (pos < expression->length()) {
        char c = (*expression)[pos];
        if (isdigit(c)) {
            pos++;
        } else if (c == '.') {
            if (dotFound) break; // Second dot stops the number
            dotFound = true;
            pos++;
        } else {
            break;
        }
    }

    std::string numStr = expression->substr(startPos, pos - startPos);
    return std::stod(numStr);
}

std::string TExpressionTree::ParseName() {
    SkipWhitespace();
    size_t startPos = pos;
    
    // Consume alphanumeric chars
    while (pos < expression->length() && isalnum((*expression)[pos])) {
        pos++;
    }

    return expression->substr(startPos, pos - startPos);
}