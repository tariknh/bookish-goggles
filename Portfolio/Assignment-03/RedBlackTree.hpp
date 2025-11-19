#pragma once
#ifndef TREDBLACKTREE_HPP
#define TREDBLACKTREE_HPP

#include <string>
#include <stdexcept>
#include <iostream>

// TEMPORARY: This is a standard BST acting as a placeholder for the RBT.
// It works perfectly for the Calculator logic.

template <typename T>
class TRedBlackTree {
private:
    struct Node {
        std::string key;
        T value;
        Node *left, *right;
        Node(std::string k, T v) : key(k), value(v), left(nullptr), right(nullptr) {}
    };

    Node* root;

    void InsertRecursive(Node*& node, std::string key, T value) {
        if (!node) {
            node = new Node(key, value);
            return;
        }
        if (key == node->key) node->value = value; // Update
        else if (key < node->key) InsertRecursive(node->left, key, value);
        else InsertRecursive(node->right, key, value);
    }

    const Node* SearchRecursive(Node* node, const std::string& key) const {
        if (!node) return nullptr;
        if (key == node->key) return node;
        if (key < node->key) return SearchRecursive(node->left, key);
        return SearchRecursive(node->right, key);
    }

    void ClearRecursive(Node* node) {
        if (!node) return;
        ClearRecursive(node->left);
        ClearRecursive(node->right);
        delete node;
    }

public:
    TRedBlackTree() : root(nullptr) {}
    ~TRedBlackTree() { ClearRecursive(root); }

    void Insert(const std::string& key, T value) {
        InsertRecursive(root, key, value);
    }

    T Search(const std::string& key) const {
        const Node* result = SearchRecursive(root, key);
        if (!result) throw std::runtime_error("Variable not found: " + key);
        return result->value;
    }
};

#endif