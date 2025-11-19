#pragma once
#ifndef TBINARYSEARCHTABLE_HPP
#define TBINARYSEARCHTABLE_HPP

#include <string>
#include <stdexcept>
#include <iostream>

// Template class so it can store doubles, ints, or anything else
template <typename T>
class TBinarySearchTable {
private:
    struct Node {
        std::string key; // The variable name (e.g., "x")
        T value;         // The value (e.g., 10.5)
        Node* left;
        Node* right;

        Node(std::string k, T v) : key(k), value(v), left(nullptr), right(nullptr) {}
    };

    Node* root;

    // --- Helper: Recursive Insert ---
    Node* InsertRecursive(Node* node, std::string key, T value) {
        // 1. Base Case: Found a spot, create new node
        if (node == nullptr) {
            return new Node(key, value);
        }

        // 2. Update value if key already exists
        if (key == node->key) {
            node->value = value;
            return node;
        }

        // 3. Recurse down the tree
        if (key < node->key) {
            node->left = InsertRecursive(node->left, key, value);
        } else {
            node->right = InsertRecursive(node->right, key, value);
        }

        return node;
    }

    // --- Helper: Recursive Search ---
    const Node* SearchRecursive(Node* node, const std::string& key) const {
        if (node == nullptr) {
            return nullptr; // Not found
        }

        if (key == node->key) {
            return node;
        }

        if (key < node->key) {
            return SearchRecursive(node->left, key);
        } else {
            return SearchRecursive(node->right, key);
        }
    }

    // --- Helper: Recursive Delete (Destructor) ---
    void ClearRecursive(Node* node) {
        if (node == nullptr) return;
        ClearRecursive(node->left);
        ClearRecursive(node->right);
        delete node;
    }

public:
    // Constructor
    TBinarySearchTable() : root(nullptr) {}

    // Destructor
    ~TBinarySearchTable() {
        ClearRecursive(root);
    }

    // Public Insert
    void Insert(const std::string& key, T value) {
        root = InsertRecursive(root, key, value);
    }

    // Public Search
    // Returns the value if found, or throws an exception if missing
    T Search(const std::string& key) const {
        const Node* result = SearchRecursive(root, key);
        if (result == nullptr) {
            throw std::runtime_error("Variable not found: " + key);
        }
        return result->value;
    }

    // Check if a key exists without throwing
    bool Contains(const std::string& key) const {
        return SearchRecursive(root, key) != nullptr;
    }
};

#endif // TBINARYSEARCHTABLE_HPP