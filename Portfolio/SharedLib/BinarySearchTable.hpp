#pragma once
#ifndef BINARY_SEARCH_TABLE_HPP
#define BINARY_SEARCH_TABLE_HPP

#include <string>
#include <iostream>

/**
 * @brief Generic Node for the Binary Search Table.
 * T is the type of value stored (e.g., double, TVertex*).
 */
template <typename T>
struct TBinarySearchTreeNode {
	std::string key;   // The identifier (e.g., "x", "Stavanger")
	T value;           // The payload
	TBinarySearchTreeNode* left;
	TBinarySearchTreeNode* right;

	TBinarySearchTreeNode(std::string aKey, T aValue)
		: key(aKey), value(aValue), left(nullptr), right(nullptr) {
	}
};

/**
 * @brief A Generic BST.
 * Manages memory for the Nodes.
 * Note: Does not assume ownership of T (if T is a pointer) to keep it compatible with primitives (double).
 */
template <typename T>
class TBinarySearchTable {
private:
	TBinarySearchTreeNode<T>* root;

	// --- Recursive Helpers ---

	void DeleteSubtree(TBinarySearchTreeNode<T>* aNode) {
		if (aNode == nullptr) return;
		// Post-order deletion
		DeleteSubtree(aNode->left);
		DeleteSubtree(aNode->right);
		delete aNode;
	}

	void InsertNode(TBinarySearchTreeNode<T>*& aNode, const std::string& aKey, T aValue) {
		if (aNode == nullptr) {
			aNode = new TBinarySearchTreeNode<T>(aKey, aValue);
		}
		else if (aKey < aNode->key) {
			InsertNode(aNode->left, aKey, aValue);
		}
		else if (aKey > aNode->key) {
			InsertNode(aNode->right, aKey, aValue);
		}
		else {
			// Key exists; update value
			aNode->value = aValue;
		}
	}

	bool SearchNode(TBinarySearchTreeNode<T>* aNode, const std::string& aKey, T& aOutValue) const {
		if (aNode == nullptr) {
			return false;
		}
		if (aKey == aNode->key) {
			aOutValue = aNode->value;
			return true;
		}
		else if (aKey < aNode->key) {
			return SearchNode(aNode->left, aKey, aOutValue);
		}
		else {
			return SearchNode(aNode->right, aKey, aOutValue);
		}
	}

public:
	TBinarySearchTable() : root(nullptr) {}

	~TBinarySearchTable() {
		DeleteSubtree(root);
	}

	void Insert(const std::string& aKey, T aValue) {
		InsertNode(root, aKey, aValue);
	}

	bool Search(const std::string& aKey, T& aOutValue) const {
		return SearchNode(root, aKey, aOutValue);
	}

	bool IsEmpty() const {
		return root == nullptr;
	}
};

#endif // BINARY_SEARCH_TABLE_HPP