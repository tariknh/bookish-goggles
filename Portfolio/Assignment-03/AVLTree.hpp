#pragma once
#ifndef TAVLTREE_HPP
#define TAVLTREE_HPP

#include <iostream>

// TEMPORARY: Standard BST placeholder. 
// Replace this logic with AVL rotations later for Extra Credit.

template <typename T>
class TAVLTree {
private:
    struct Node {
        T data;
        Node *left, *right;
        Node(T val) : data(val), left(nullptr), right(nullptr) {}
    };

    Node* root;

    void InsertRecursive(Node*& node, T key) {
        if (!node) {
            node = new Node(key);
            return;
        }
        if (key < node->data) InsertRecursive(node->left, key);
        else if (key > node->data) InsertRecursive(node->right, key);
    }

    // Basic BST Delete (Not AVL balanced yet)
    Node* DeleteRecursive(Node* root, T key) {
        if (root == nullptr) return root;

        if (key < root->data) root->left = DeleteRecursive(root->left, key);
        else if (key > root->data) root->right = DeleteRecursive(root->right, key);
        else {
            if (root->left == nullptr) {
                Node* temp = root->right;
                delete root;
                return temp;
            } else if (root->right == nullptr) {
                Node* temp = root->left;
                delete root;
                return temp;
            }
            Node* temp = MinValueNode(root->right);
            root->data = temp->data;
            root->right = DeleteRecursive(root->right, temp->data);
        }
        return root;
    }

    Node* MinValueNode(Node* node) {
        Node* current = node;
        while (current && current->left != nullptr) current = current->left;
        return current;
    }

    void InOrder(Node* node) {
        if (node) {
            InOrder(node->left);
            std::cout << node->data << " ";
            InOrder(node->right);
        }
    }

    void ClearRec(Node* node) {
        if(node) { ClearRec(node->left); ClearRec(node->right); delete node; }
    }

public:
    TAVLTree() : root(nullptr) {}
    ~TAVLTree() { ClearRec(root); }

    void Insert(T key) { InsertRecursive(root, key); }
    void Remove(T key) { root = DeleteRecursive(root, key); }
    
    void PrintTree() { InOrder(root); std::cout << std::endl; }
};

#endif