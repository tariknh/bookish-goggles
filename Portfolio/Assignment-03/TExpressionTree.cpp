#include "TExpressionTree.h"
#include <cctype>  
#include <cmath>   
#include <stdexcept> 
#include "RedBlackTree.hpp"



TExpressionTree::TExpressionTree() 
    : root(nullptr), expression(nullptr), pos(0) {
}

TExpressionTree::~TExpressionTree() {
    DeleteSubtree(root);
}

void TExpressionTree::DeleteSubtree(TExpressionNode* aNode) {
    if (!aNode) return;

    DeleteSubtree(aNode->left);
    DeleteSubtree(aNode->right);

    delete aNode;
}



bool TExpressionTree::BuildTree(const std::string& aExpression) {
    DeleteSubtree(root);
    root = nullptr;

    expression = &aExpression;
    pos = 0;

    try {
        root = ParseExpression();

        SkipWhitespace();
        if (Peek() != '\0') {
            DeleteSubtree(root);
            root = nullptr;
            return false;
        }
        return true;

    } catch (...) {
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


double TExpressionTree::EvaluateNode(TExpressionNode* aNode, const TRedBlackTree<double>& aSymTable) const {
    if (!aNode) return 0.0;

    if (aNode->type == ENodeType::OPERAND) {
        return aNode->value;
    }

    if (aNode->type == ENodeType::VARIABLE) {
        try {
            return aSymTable.Search(aNode->name); 
        } catch (...) {
            throw std::runtime_error("Undefined variable: " + aNode->name);
        }
    }

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

    if (aNode->type == ENodeType::FUNCTION) {
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


TExpressionNode* TExpressionTree::ParseExpression() {
    TExpressionNode* left = ParseTerm();

    while (true) {
        SkipWhitespace();
        char c = Peek();
        if (c == '+' || c == '-') {
            Get(); 
            TExpressionNode* right = ParseTerm();
            
            TExpressionNode* parent = new TExpressionNode(c, left, right);
            left = parent; // The new parent becomes the left child for the next op
        } else {
            break;
        }
    }
    return left;
}

TExpressionNode* TExpressionTree::ParseTerm() {
    TExpressionNode* left = ParseFactor();

    while (true) {
        SkipWhitespace();
        char c = Peek();
        if (c == '*' || c == '/' || c == '^') {
            Get(); // We take the operator
            TExpressionNode* right = ParseFactor();

            TExpressionNode* parent = new TExpressionNode(c, left, right);
            left = parent;
        } else {
            break;
        }
    }
    return left;
}

TExpressionNode* TExpressionTree::ParseFactor() {
    SkipWhitespace();
    char c = Peek();

    if (Match('(')) {
        TExpressionNode* node = ParseExpression();
        if (!Match(')')) {
            // Clean up if parenthesis mismatch
            DeleteSubtree(node);
            throw std::runtime_error("Missing closing parenthesis");
        }
        return node;
    }

    if (isdigit(c) || c == '.') {
        double val = ParseNumber();
        return new TExpressionNode(val);
    }

    if (isalpha(c)) {
        std::string name = ParseName();
        
        SkipWhitespace();
        if (Peek() == '(') {
            // Means its a function
            Match('('); // Consume '('
            TExpressionNode* arg = ParseExpression(); // Argument is the expression inside
            if (!Match(')')) {
                DeleteSubtree(arg);
                throw std::runtime_error("Missing closing parenthesis for function");
            }
            
            TExpressionNode* funcNode = new TExpressionNode(name, ENodeType::FUNCTION);
            funcNode->left = arg; 
            return funcNode;
        } 
        else {
            // It's just a variable
            return new TExpressionNode(name, ENodeType::VARIABLE);
        }
    }

    if (c == '-') {
        Get(); // Take the minus
        TExpressionNode* right = ParseFactor();
        // Represent -X as (0 - X)
        return new TExpressionNode('-', new TExpressionNode(0.0), right);
    }

    throw std::runtime_error("Unexpected character in expression");
}

double TExpressionTree::ParseNumber() {
    SkipWhitespace();
    size_t startPos = pos;
    
    // Take digits and optional dot
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
    
    while (pos < expression->length() && isalnum((*expression)[pos])) {
        pos++;
    }

    return expression->substr(startPos, pos - startPos);
}