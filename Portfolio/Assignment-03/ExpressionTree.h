#pragma once
#ifndef TEXPRESSIONTREE_H
#define TEXPRESSIONTREE_H

#include <string>
#include <iostream>
#include "BinarySearchTable.hpp" // Include the new template

// Enum to identify node types
enum class ENodeType {
	OPERAND,    // Numbers
	OPERATOR,   // +, -, *, /, ^
	VARIABLE,   // x, y
	FUNCTION    // sin, cos, etc.
};

// The Tree Node (Heap Allocated)
struct TExpressionNode {
	ENodeType type;
	double value;       // Used for OPERAND
	char op;            // Used for OPERATOR
	std::string name;   // Used for VARIABLE or FUNCTION

	TExpressionNode* left;
	TExpressionNode* right;

	// Constructor Helpers
	TExpressionNode(double aVal)
		: type(ENodeType::OPERAND), value(aVal), op(0), left(nullptr), right(nullptr) {
	}

	TExpressionNode(char aOp, TExpressionNode* aLeft, TExpressionNode* aRight)
		: type(ENodeType::OPERATOR), value(0), op(aOp), name(""), left(aLeft), right(aRight) {
	}

	TExpressionNode(std::string aName, ENodeType aType)
		: type(aType), value(0), op(0), name(aName), left(nullptr), right(nullptr) {
	}
};

/**
 * @brief Handles parsing and evaluating math expressions.
 */
class TExpressionTree {
private:
	TExpressionNode* root;
	const std::string* expression;
	int pos;

	// --- Helpers ---
	void DeleteSubtree(TExpressionNode* aNode);

	// --- Parser (Recursive Descent) ---
	char Peek();
	char Get();
	bool Match(char aExpected);
	void SkipWhitespace();

	// Grammar functions
	TExpressionNode* ParseExpression();
	TExpressionNode* ParseTerm();
	TExpressionNode* ParseFactor();

	double ParseNumber();
	std::string ParseName();

	// --- Evaluation ---
	// Note: Now passing the Templated Table
	double EvaluateNode(TExpressionNode* aNode, const TBinarySearchTable<double>& aSymTable) const;

public:
	TExpressionTree();
	~TExpressionTree();

	bool BuildTree(const std::string& aExpression);

	// Pass the symbol table by const reference for lookups
	double Evaluate(const TBinarySearchTable<double>& aSymTable) const;
};

#endif // TEXPRESSIONTREE_H