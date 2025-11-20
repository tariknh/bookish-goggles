#pragma once
#ifndef TBINARYSEARCHTABLE_HPP
#define TBINARYSEARCHTABLE_HPP

#include <string>
#include <stdexcept>
#include <iostream>

// Template class so it can store doubles, ints, or anything else WE MIGht but in the calculator
template <typename T>
class TBinarySearchTable {
private:
    struct Node {
        std::string key; // Fore example x
        T value;         // The value we set the var to
        Node* left;
        Node* right;

        Node(std::string k, T v) : key(k), value(v), left(nullptr), right(nullptr) {}
    };

    Node* root;

    Node* InsertRecursive(Node* node, std::string key, T value) {
        //base case
        if (node == nullptr) {
            return new Node(key, value);
        }

        if (key == node->key) {
            node->value = value;
            return node;
        }

        if (key < node->key) {
            node->left = InsertRecursive(node->left, key, value);
        } else {
            node->right = InsertRecursive(node->right, key, value);
        }

        return node;
    }

    void ListallHelper(){

    }
    void ListAll() const {
        ListAllHelper(root);
    }
    const Node* SearchRecursive(Node* node, const std::string& key) const {
        if (node == nullptr) {
            return nullptr; 
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

    // This is post order deletion
    void ClearRecursive(Node* node) {
        if (node == nullptr) return;
        ClearRecursive(node->left);
        ClearRecursive(node->right);
        delete node;
    }

public:
    TBinarySearchTable() : root(nullptr) {}

    ~TBinarySearchTable() {
        ClearRecursive(root);
    }

    void Insert(const std::string& key, T value) {
        root = InsertRecursive(root, key, value);
    }

    // The public search. Returns the value if found, or throws an exception if missing
    T Search(const std::string& key) const {
        const Node* result = SearchRecursive(root, key);
        if (result == nullptr) {
            throw std::runtime_error("Variable not found: " + key);
        }
        return result->value;
    }

    bool Contains(const std::string& key) const {
        return SearchRecursive(root, key) != nullptr;
    }
};

#endif // TBINARYSEARCHTABLE_HPP