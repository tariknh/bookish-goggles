#pragma once

#ifndef OPTION2_H
#define OPTION2_H

#include <string>

struct TEmployee
{
    std::string firstName;
    std::string lastName;
    int employeeID{ 0 };
};

struct TTreeNode
{
    int key;
    TEmployee* data;
    TTreeNode* left;
    TTreeNode* right;

    TTreeNode(int aKey, TEmployee* aData)
        : key(aKey), data(aData), left(nullptr), right(nullptr)
    {
    }
};

class BinarySearchTree
{
public:
    BinarySearchTree();
    ~BinarySearchTree();

    TEmployee* Search(int key) const;
    TTreeNode* Insert(int key, TEmployee* data);
    void Delete(int key);

private:
    TTreeNode* root;

    TTreeNode* InsertNode(TTreeNode* node, int key, TEmployee* data);
    TTreeNode* DeleteNode(TTreeNode* node, int key);
    TTreeNode* FindMin(TTreeNode* node) const;
    void DestroyNodes(TTreeNode* node);
};

class AVLNode {
public:
    TEmployee* data;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(TEmployee* p)
        : data(p), left(nullptr), right(nullptr), height(1) {}
};


int RunApp();


#endif // OPTION2_H
