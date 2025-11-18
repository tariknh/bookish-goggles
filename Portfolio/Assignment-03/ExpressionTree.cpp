#include "ExpressionTree.h"
#include <cctype> // isdigit, isalpha
#include <cmath>  // pow, sin, cos, etc.
#include <iostream>

// --- Constructor / Destructor ---

TExpressionTree::TExpressionTree()
	: root(nullptr), expression(nullptr), pos(0)
{
}

TExpressionTree::~TExpressionTree()
{
	DeleteSubtree(root);
}

void TExpressionTree::DeleteSubtree(TExpressionNode* aNode)
{
	if (!aNode) return;

	// Recursive Post-Order Deletion (Children first, then parent)
	DeleteSubtree(aNode->left);
	DeleteSubtree(aNode->right);

	// Delete the current node
	delete aNode;
}

// --- Public Interface ---

bool TExpressionTree::BuildTree(const std::string& aExpression)
{
	// 1. Clear any existing tree
	DeleteSubtree(root);
	root = nullptr;

	// 2. Reset Parser State
	expression = &aExpression;
	pos = 0;

	// 3. Start Recursive Descent
	try
	{
		root = ParseExpression();

		// Check if we consumed the whole string (excluding trailing spaces)
		SkipWhitespace();
		if (pos < (int)expression->length()) {
			// We stopped parsing but text remains (e.g. "5 + 5 )")
			std::cerr << "Syntax Error: Unexpected character at end of expression." << std::endl;
			DeleteSubtree(root);
			root = nullptr;
			return false;
		}
		return true;
	}
	catch (const std::exception&)
	{
		// Clean up partial tree on failure
		DeleteSubtree(root);
		root = nullptr;
		return false;
	}
}

double TExpressionTree::Evaluate(const TBinarySearchTable<double>& aSymTable) const
{
	if (!root) return 0.0;
	return EvaluateNode(root, aSymTable);
}

// --- Parser Helpers ---

char TExpressionTree::Peek()
{
	SkipWhitespace();
	if (pos >= (int)expression->length()) return '\0';
	return (*expression)[pos];
}

char TExpressionTree::Get()
{
	char c = Peek();
	if (pos < (int)expression->length()) pos++;
	return c;
}

bool TExpressionTree::Match(char aExpected)
{
	if (Peek() == aExpected) {
		Get();
		return true;
	}
	return false;
}

void TExpressionTree::SkipWhitespace()
{
	while (pos < (int)expression->length() && std::isspace((*expression)[pos])) {
		pos++;
	}
}

// --- Grammar Implementation (Recursive Descent) ---

// Expression -> Term { +|- Term }
TExpressionNode* TExpressionTree::ParseExpression()
{
	TExpressionNode* left = ParseTerm();

	while (true)
	{
		char op = Peek();
		if (op == '+' || op == '-')
		{
			Get(); // Consume operator
			TExpressionNode* right = ParseTerm();

			// Build tree upwards: New Operator becomes the parent
			left = new TExpressionNode(op, left, right);
		}
		else
		{
			break;
		}
	}
	return left;
}

// Term -> Factor { *|/ Factor }
TExpressionNode* TExpressionTree::ParseTerm()
{
	TExpressionNode* left = ParseFactor();

	while (true)
	{
		char op = Peek();
		if (op == '*' || op == '/')
		{
			Get(); // Consume operator
			TExpressionNode* right = ParseFactor();

			// Build tree upwards
			left = new TExpressionNode(op, left, right);
		}
		else
		{
			break;
		}
	}
	return left;
}

// Factor -> (Expr) | Number | Variable | Function | -Factor | Base ^ Power
TExpressionNode* TExpressionTree::ParseFactor()
{
	TExpressionNode* node = nullptr;
	char c = Peek();

	// 1. Handle Parentheses: ( Expression )
	if (Match('('))
	{
		node = ParseExpression();
		if (!Match(')')) {
			// In a real app, throw exception or handle error gracefully
			std::cerr << "Syntax Error: Missing ')'" << std::endl;
		}
	}
	// 2. Handle Unary Minus: -Factor (e.g. -5)
	else if (c == '-')
	{
		Get(); // Consume '-'
		TExpressionNode* right = ParseFactor();
		// Represent -x as (0 - x) for simplicity in evaluation
		node = new TExpressionNode('-', new TExpressionNode(0.0), right);
	}
	// 3. Handle Numbers
	else if (isdigit(c) || c == '.')
	{
		double val = ParseNumber();
		node = new TExpressionNode(val);
	}
	// 4. Handle Variables or Functions
	else if (isalpha(c))
	{
		std::string name = ParseName();

		if (Match('('))
		{
			// It is a Function call: sin( ... )
			TExpressionNode* arg = ParseExpression();
			if (!Match(')')) {
				std::cerr << "Syntax Error: Missing ')' after function argument" << std::endl;
			}
			// Create Function Node
			node = new TExpressionNode(name, ENodeType::FUNCTION);
			node->left = arg; // Store argument in left child
		}
		else
		{
			// It is a Variable
			node = new TExpressionNode(name, ENodeType::VARIABLE);
		}
	}
	else
	{
		// Unexpected character (return 0 node to prevent crash, or nullptr)
		Get();
		return new TExpressionNode(0.0);
	}

	// 5. Handle Exponentiation (^)
	// Power binds tighter than * / but looser than parens/numbers.
	// We check for it here to attach it to the factor we just parsed.
	if (Match('^'))
	{
		TExpressionNode* right = ParseFactor(); // Recursively get the exponent
		node = new TExpressionNode('^', node, right);
	}

	return node;
}

double TExpressionTree::ParseNumber()
{
	std::string numStr;
	while (pos < (int)expression->length() && (isdigit((*expression)[pos]) || (*expression)[pos] == '.'))
	{
		numStr += (*expression)[pos];
		pos++;
	}
	try {
		return std::stod(numStr);
	}
	catch (...) {
		return 0.0;
	}
}

std::string TExpressionTree::ParseName()
{
	std::string name;
	while (pos < (int)expression->length() && isalpha((*expression)[pos]))
	{
		name += (*expression)[pos];
		pos++;
	}
	return name;
}

// --- Evaluation Logic ---

double TExpressionTree::EvaluateNode(TExpressionNode* aNode, const TBinarySearchTable<double>& aSymTable) const
{
	if (!aNode) return 0.0;

	// Post-order traversal implicitly happens via recursion

	if (aNode->type == ENodeType::OPERAND)
	{
		return aNode->value;
	}
	else if (aNode->type == ENodeType::VARIABLE)
	{
		double val = 0.0;
		if (aSymTable.Search(aNode->name, val)) {
			return val;
		}
		else {
			std::cerr << "Warning: Undefined variable '" << aNode->name << "'. Using 0." << std::endl;
			return 0.0;
		}
	}
	else if (aNode->type == ENodeType::OPERATOR)
	{
		double leftVal = EvaluateNode(aNode->left, aSymTable);
		double rightVal = EvaluateNode(aNode->right, aSymTable);

		switch (aNode->op)
		{
		case '+': return leftVal + rightVal;
		case '-': return leftVal - rightVal;
		case '*': return leftVal * rightVal;
		case '/':
			if (rightVal == 0.0) {
				std::cerr << "Error: Division by zero." << std::endl;
				return 0.0;
			}
			return leftVal / rightVal;
		case '^': return std::pow(leftVal, rightVal);
		default: return 0.0;
		}
	}
	else if (aNode->type == ENodeType::FUNCTION)
	{
		// Function argument is stored in 'left'
		double arg = EvaluateNode(aNode->left, aSymTable);

		if (aNode->name == "sin") return std::sin(arg);
		if (aNode->name == "cos") return std::cos(arg);
		if (aNode->name == "tan") return std::tan(arg);
		if (aNode->name == "log") return std::log(arg); // Natural log
		if (aNode->name == "exp") return std::exp(arg);
		if (aNode->name == "sqrt") return std::sqrt(arg);

		std::cerr << "Warning: Unknown function '" << aNode->name << "'." << std::endl;
		return 0.0;
	}

	return 0.0;
}