#pragma once
#ifndef TAVLTREE_HPP
#define TAVLTREE_HPP

#include <algorithm>
#include <iostream>

template <typename T>
class TAVLTree {
private:
    struct Node {
        T data;
        int height;
        Node *left, *right;
        Node(T val) : data(val), height(1), left(nullptr), right(nullptr) {}
    };

    Node* root;

    int Height(Node* N) {
        if (N == nullptr) return 0;
        return N->height;
    }

    int GetBalance(Node* N) {
        if (N == nullptr) return 0;
        return Height(N->left) - Height(N->right);
    }

    Node* RightRotate(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;
        x->right = y;
        y->left = T2;
        y->height = std::max(Height(y->left), Height(y->right)) + 1;
        x->height = std::max(Height(x->left), Height(x->right)) + 1;
        return x;
    }

    Node* LeftRotate(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;
        y->left = x;
        x->right = T2;
        x->height = std::max(Height(x->left), Height(x->right)) + 1;
        y->height = std::max(Height(y->left), Height(y->right)) + 1;
        return y;
    }

    Node* InsertRec(Node* node, T key) {
        if (node == nullptr) return new Node(key);

        if (key < node->data) node->left = InsertRec(node->left, key);
        else if (key > node->data) node->right = InsertRec(node->right, key);
        else return node;

        node->height = 1 + std::max(Height(node->left), Height(node->right));
        int balance = GetBalance(node);

        // Here we do rotations based on the result from getbalance. 
        if (balance > 1 && key < node->left->data) return RightRotate(node);
        if (balance < -1 && key > node->right->data) return LeftRotate(node);
        if (balance > 1 && key > node->left->data) {
            node->left = LeftRotate(node->left);
            return RightRotate(node);
        }
        if (balance < -1 && key < node->right->data) {
            node->right = RightRotate(node->right);
            return LeftRotate(node);
        }
        return node;
    }

    Node* MinValueNode(Node* node) {
        Node* current = node;
        while (current->left != nullptr) current = current->left;
        return current;
    }

    //Avl delete, recursively.
    Node* DeleteRec(Node* root, T key) {
        if (root == nullptr) return root;

        if (key < root->data) root->left = DeleteRec(root->left, key);
        else if (key > root->data) root->right = DeleteRec(root->right, key);
        else {
            if ((root->left == nullptr) || (root->right == nullptr)) {
                Node* temp = root->left ? root->left : root->right;
                if (temp == nullptr) {
                    temp = root;
                    root = nullptr;
                } else *root = *temp;
                delete temp;
            } else {
                Node* temp = MinValueNode(root->right);
                root->data = temp->data;
                root->right = DeleteRec(root->right, temp->data);
            }
        }

        if (root == nullptr) return root;

        root->height = 1 + std::max(Height(root->left), Height(root->right));

        int balance = GetBalance(root);

        // Left Left rotation
        if (balance > 1 && GetBalance(root->left) >= 0) return RightRotate(root);

        // Left Right rotation
        if (balance > 1 && GetBalance(root->left) < 0) {
            root->left = LeftRotate(root->left);
            return RightRotate(root);
        }

        // Right Right
        if (balance < -1 && GetBalance(root->right) <= 0) return LeftRotate(root);

        // Right Left
        if (balance < -1 && GetBalance(root->right) > 0) {
            root->right = RightRotate(root->right);
            return LeftRotate(root);
        }

        return root;
    }

    void ClearRec(Node* node) {
        if(node) { ClearRec(node->left); ClearRec(node->right); delete node; }
    }

    void InOrder(Node* node) {
        if (node) {
            InOrder(node->left);
            std::cout << node->data << " ";
            InOrder(node->right);
        }
    }

public:
    TAVLTree() : root(nullptr) {}
    ~TAVLTree() { ClearRec(root); }

    void Insert(T key) { root = InsertRec(root, key); }
    void Remove(T key) { root = DeleteRec(root, key); }
    void PrintTree() { InOrder(root); std::cout << std::endl; }
};

#endif