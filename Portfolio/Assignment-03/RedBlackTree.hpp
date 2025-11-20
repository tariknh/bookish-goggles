#pragma once
#ifndef TREDBLACKTREE_HPP
#define TREDBLACKTREE_HPP

#include <string>
#include <stdexcept>
#include <iostream>

template <typename T>
class TRedBlackTree {
private:
    enum Color { RED, BLACK };

    struct Node {
        std::string key;
        T value;
        Color color;
        Node *left, *right, *parent;

        Node(std::string k, T v, Color c = RED)
            : key(k), value(v), color(c), left(nullptr), right(nullptr), parent(nullptr) {}
    };

    Node* root;

    //function to rotate left
    void RotateLeft(Node* x) {
        Node* y = x->right;
        x->right = y->left;

        if (y->left != nullptr) {
            y->left->parent = x;
        }

        y->parent = x->parent;

        if (x->parent == nullptr) {
            root = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }

        y->left = x;
        x->parent = y;
    }

    //function to rotate right
    void RotateRight(Node* y) {
        Node* x = y->left;
        y->left = x->right;

        if (x->right != nullptr) {
            x->right->parent = y;
        }

        x->parent = y->parent;

        if (y->parent == nullptr) {
            root = x;
        } else if (y == y->parent->left) {
            y->parent->left = x;
        } else {
            y->parent->right = x;
        }

        x->right = y;
        y->parent = x;
    }

    // Fix Red-Black Tree properties after insertion
    void FixInsert(Node* k) {
        while (k != root && k->parent->color == RED) {
            if (k->parent == k->parent->parent->left) {
                Node* uncle = k->parent->parent->right;

                // Case 1: Uncle is RED
                if (uncle != nullptr && uncle->color == RED) {
                    k->parent->color = BLACK;
                    uncle->color = BLACK;
                    k->parent->parent->color = RED;
                    k = k->parent->parent;
                } else {
                    // Case 2: Uncle is BLACK and k is right child
                    if (k == k->parent->right) {
                        k = k->parent;
                        RotateLeft(k);
                    }
                    // Case 3: Uncle is BLACK and k is left child
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    RotateRight(k->parent->parent);
                }
            } else {
                Node* uncle = k->parent->parent->left;

                // Case 1: Uncle is RED
                if (uncle != nullptr && uncle->color == RED) {
                    k->parent->color = BLACK;
                    uncle->color = BLACK;
                    k->parent->parent->color = RED;
                    k = k->parent->parent;
                } else {
                    // Case 2: Uncle is BLACK and k is left child
                    if (k == k->parent->left) {
                        k = k->parent;
                        RotateRight(k);
                    }
                    // Case 3: Uncle is BLACK and k is right child
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    RotateLeft(k->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }

    // Returns the pointer to the NEWLY created node (so we can fix it).
    // Returns nullptr if we just updated an existing key.
    Node* BSTInsert(std::string key, T value) {
        Node* current = root;
        Node* parent = nullptr;

        // 1. Traverse down to find the insert spot
        while (current != nullptr) {
            parent = current;
            
            if (key == current->key) {
                // Key exists! Just update value and return nullptr (no fixup needed)
                current->value = value;
                return nullptr;
            }
            else if (key < current->key) {
                current = current->left;
            }
            else {
                current = current->right;
            }
        }

        // 2. Create the new node
        // Note: We pass 'parent' to the constructor so the link is made immediately
        Node* newNode = new Node(key, value, RED);
        newNode->parent = parent; 

        // 3. Link the parent to the new node
        if (parent == nullptr) {
            // Tree was empty, new node is root
            root = newNode;
        } 
        else if (key < parent->key) {
            parent->left = newNode;
        } 
        else {
            parent->right = newNode;
        }

        return newNode;
    }

    // Search helper
    const Node* SearchRecursive(Node* node, const std::string& key) const {
        if (node == nullptr) return nullptr;
        if (key == node->key) return node;
        if (key < node->key) return SearchRecursive(node->left, key);
        return SearchRecursive(node->right, key);
    }

    // Clear tree helper
    void ClearRecursive(Node* node) {
        if (node == nullptr) return;
        ClearRecursive(node->left);
        ClearRecursive(node->right);
        delete node;
    }

public:
    TRedBlackTree() : root(nullptr) {}

    ~TRedBlackTree() {
        ClearRecursive(root);
    }

    // Insert a key-value pair
    void Insert(const std::string& key, T value) {
        if (root == nullptr) {
            root = new Node(key, value, BLACK);
            return;
        }

        Node* newNode = BSTInsert( key, value);

        // If BSTInsert returns nullptr, the key already existed and was updated
        if (newNode == nullptr) {
            return;
        }

        
        FixInsert(newNode);
    }

    // Search for a key and return its value
    T Search(const std::string& key) const {
        const Node* result = SearchRecursive(root, key);
        if (result == nullptr) {
            throw std::runtime_error("Variable not found: " + key);
        }
        return result->value;
    }

    // Check if tree is empty
    bool IsEmpty() const {
        return root == nullptr;
    }

    
    void PrintInOrder() const {
        PrintInOrderHelper(root);
        std::cout << std::endl;
    }

private:
    void PrintInOrderHelper(Node* node) const {
        if (node == nullptr) return;
        PrintInOrderHelper(node->left);
        std::cout << node->key << "=" << node->value << " ";
        PrintInOrderHelper(node->right);
    }
};

#endif