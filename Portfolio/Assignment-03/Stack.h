#pragma once
#include <iostream>

template <typename T>
class Stack {
private:
    struct Node {
        T data;
        Node* next;
        Node(T val) : data(val), next(nullptr) {}
    };

    Node* topNode;
    int count;

public:
    Stack() : topNode(nullptr), count(0) {}

    ~Stack() {
        clear();
    }

    void push(T val) {
        Node* newNode = new Node(val);
        newNode->next = topNode;
        topNode = newNode;
        count++;
    }

    void pop() {
        if (isEmpty()) return;
        Node* temp = topNode;
        topNode = topNode->next;
        delete temp;
        count--;
    }

    // Returns the data at the top without removing it
    T peek() {
        if (isEmpty()) {
            // In a real scenario, you might throw an exception here.
            // For this assignment, assume we check isEmpty() before peeking.
            // Returning a default-constructed T is a safe fallback.
            return T(); 
        }
        return topNode->data;
    }

    bool isEmpty() {
        return topNode == nullptr;
    }

    void clear() {
        while (!isEmpty()) {
            pop();
        }
    }
    
    int size() {
        return count;
    }
};