#pragma once
#ifndef TEXPRESSIONTREE_H
#define TEXPRESSIONTREE_H

#include <string>
#include <iostream>
#include <vector>

#include "RedBlackTree.hpp" 

enum class ENodeType {
    OPERAND,    // Numbers
    OPERATOR,   // +, -, *, /, ^
    VARIABLE,   // x, y
    FUNCTION    // sin, cos, etc.
};

struct TExpressionNode {
    ENodeType type;
    double value;       
    char op;            
    std::string name;   

    TExpressionNode* left;
    TExpressionNode* right;

    TExpressionNode(double aVal)
        : type(ENodeType::OPERAND), value(aVal), op(0), left(nullptr), right(nullptr) {}

    TExpressionNode(char aOp, TExpressionNode* aLeft, TExpressionNode* aRight)
        : type(ENodeType::OPERATOR), value(0), op(aOp), name(""), left(aLeft), right(aRight) {}

    TExpressionNode(std::string aName, ENodeType aType)
        : type(aType), value(0), op(0), name(aName), left(nullptr), right(nullptr) {}
};

class TExpressionTree {
private:
    TExpressionNode* root;
    const std::string* expression;
    size_t pos;

    void DeleteSubtree(TExpressionNode* aNode);

    char Peek();
    char Get();
    bool Match(char aExpected);
    void SkipWhitespace();

    TExpressionNode* ParseExpression();
    TExpressionNode* ParseTerm();
    TExpressionNode* ParseFactor();

    double ParseNumber();
    std::string ParseName();

    double EvaluateNode(TExpressionNode* aNode, const TRedBlackTree<double>& aSymTable) const;

public:
    TExpressionTree();
    ~TExpressionTree();

    bool BuildTree(const std::string& aExpression);

    double Evaluate(const TRedBlackTree<double>& aSymTable) const;
};

#endif // TEXPRESSIONTREE_H