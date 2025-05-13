#pragma once

#include <cstdint>
#include "alg_concepts.hpp"
#include "binary_tree.hpp"
#include "common.hpp"

namespace alg {

namespace detail {

/**
 * Helper function to check if a binary tree is a binary search tree.
 * It performs an in-order traversal and checks if the values are sorted in ascending order.
 * This check is done by comparing the value of the previously visited node with the current node.
 */
template <typename T>
bool checkBSTHelper(const TreeNode<T>* root, const TreeNode<T>*& prev) {
    if (!root) return true;
    if (!checkBSTHelper(root->left, prev)) return false;
    if (prev && prev->val >= root->val) return false;
    prev = root;
    return checkBSTHelper(root->right, prev);
}

/**
 * Helper function to get the kth smallest element in a binary search tree.
 */
template <typename T>
const TreeNode<T>* getBSTKthMinHelper(const TreeNode<T>* root, std::size_t& k) {
    if (!root) return nullptr;
    const TreeNode<T>* left = getBSTKthMinHelper(root->left, k);
    if (left) return left;
    if (!k--) return root;
    return getBSTKthMinHelper(root->right, k);
}

} // namespace detail

/**
 * Converts a binary tree into a binary search tree, keeping the original tree structure.
 * The function flattens the tree into a vector and sorts it in ascending order.
 * Then it uses in-order traversal to reconstruct the tree.
 */
template <typename T>
TreeNode<T>* toBST(TreeNode<T>* root) {
    std::vector<T> vec = toVector(root);
    std::sort(vec.begin(), vec.end());
    index i = 0;
    inorderTreeApply<T>(root, [&vec, &i](T& val) { val = vec[i++]; });
    return root;
}

/**
 * Checks if a binary tree is a binary search tree.
 * A binary tree is a BST if it respects the binary search tree property.
 */
template <typename T>
bool checkBST(const TreeNode<T>* root) {
    const TreeNode<T>* prev = nullptr;
    return detail::checkBSTHelper(root, prev);
}

/**
 * Inserts a value into a binary search tree using recursion.
 * If the value already exists, the function does nothing.
 * The new node is always inserted as a leaf.
 */
template <typename T>
TreeNode<T>* insertBSTRec(TreeNode<T>* root, const T& val) {
    if (!root) root = new TreeNode<T>{.val = val};
    if (root->val == val) return root;
    if (val < root->val) root->left = insertBSTRec(root->left, val);
    else root->right = insertBSTRec(root->right, val);
    return root;
}

/**
 * Inserts a value into a binary search tree using an iterative loop.
 * If the value already exists, the function does nothing.
 * The new node is always inserted as a leaf.
 */
template <typename T>
TreeNode<T>* insertBSTIter(TreeNode<T>* root, const T& val) {
    TreeNode<T>* prev = nullptr;
    TreeNode<T>* curr = root;
    while (curr) {
        if (curr->val == val) return root;
        prev = curr;
        curr = (val < curr->val) ? curr->left : curr->right;
    }
    if (!prev) return new TreeNode<T>{.val = val};
    if (val < prev->val) prev->left = new TreeNode<T>{.val = val};
    else prev->right = new TreeNode<T>{.val = val};
    return root;
}

/**
 * Removes a value from a binary search tree using recursion.
 * If the value is not found, the function does nothing.
 */
template <typename T>
TreeNode<T>* removeBSTRec(TreeNode<T>* root, const T& val) {
    if (!root) return root;
    if (val < root->val) root->left = removeBSTRec(root->left, val);
    else if (val > root->val) root->right = removeBSTRec(root->right, val);
    else {
        // The node to remove has 0 children or only the right child
        if (!root->left) {
            TreeNode<T>* right = root->right;
            delete root;
            return right;
        }
        // The node to remove has only the left child
        if (!root->right) {
            TreeNode<T>* left = root->left;
            delete root;
            return left;
        }
        // The node to remove has both children
        const TreeNode<T>* min = getBSTMin(root->right);
        root->val = min->val;
        root->right = removeBSTRec(root->right, min->val);
    }
    return root;
}

/**
 * Removes a value from a binary search tree using an iterative loop.
 * If the value is not found, the function does nothing.
 */
template <typename T>
TreeNode<T>* removeBSTIter(TreeNode<T>* root, const T& val) {
    TreeNode<T>* prev = nullptr;
    TreeNode<T>* curr = root;
    while (curr && curr->val != val) {
        prev = curr;
        curr = (val < curr->val) ? curr->left : curr->right;
    }
    // The value is not present
    if (!curr) return root;
    // The node to remove has 0 or 1 children
    if (!curr->left || !curr->right) {
        TreeNode<T>* child = curr->left ? curr->left : curr->right;
        // The node to remove is the root
        if (!prev) { delete curr; return child; }
        if (val < prev->val) prev->left = child;
        else prev->right = child;
        delete curr;
    }
    // The node to remove has 2 children
    else {
        TreeNode<T>* parent = nullptr;
        TreeNode<T>* min = curr->right;
        while (min->left) {
            parent = min;
            min = min->left;
        }
        // Substitute the value of the node to remove with the value of the successor
        curr->val = min->val;
        // The parent of the successor node is not the node to remove
        if (parent) parent->left = min->right;
        else curr->right = min->right;
        curr->val = min->val;
        delete min;
    }
    return root;
}


/**
 * Finds a node with a given value in a binary search tree using recursion.
 * If the node is not found, returns nullptr.
 */
template <typename T>
const TreeNode<T>* findBSTRec(const TreeNode<T>* root, const T& val) {
    if (!root || root->val == val) return root;
    return (val < root->val) 
        ? findBSTRec(root->left, val) 
        : findBSTRec(root->right, val);
}

/**
 * Finds a node with a given value in a binary search tree using an iterative loop.
 * If the node is not found, returns nullptr.
 */
template <typename T>
const TreeNode<T>* findBSTIter(const TreeNode<T>* root, const T& val) {
    while (root && root->val != val)
        root = (val < root->val) ? root->left : root->right;
    return root;
}

/**
 * Finds the node with the minimum value in a binary search tree using an iterative loop.
 * If the tree is empty, returns nullptr.
 */
template <typename T>
const TreeNode<T>* getBSTMin(const TreeNode<T>* root) {
    if (!root) return nullptr;
    while (root->left) root = root->left;
    return root;
}

/**
 * Finds the node with the maximum value in a binary search tree using an iterative loop.
 * If the tree is empty, returns nullptr.
 */
template <typename T>
const TreeNode<T>* getBSTMax(const TreeNode<T>* root) {
    if (!root) return nullptr;
    while (root->right) root = root->right;
    return root;
}

/**
 * Finds the kth smallest value in a binary search tree using an iterative loop.
 * The counting starts from 0, which equals to the minimum element in the tree.
 */
template <typename T>
const TreeNode<T>* getBSTKthMin(const TreeNode<T>* root, std::size_t k) {
    return detail::getBSTKthMinHelper(root, k);
}

/**
 * Finds the lowest common ancestor of two nodes in a binary search tree.
 * This uses an iterative loop.
 */
template <typename T>
const TreeNode<T>* lcaBST(const TreeNode<T>* root, const TreeNode<T>* n1, const TreeNode<T>* n2) {
    while (root) {
        // If the root has value less than both nodes, go left
        if (root->val > n1->val && root->val > n2->val) root = root->left;
        // If the root has value greater than both nodes, go right
        else if (root->val < n1->val && root->val < n2->val) root = root->right;
        else break;
    }
    return root;
}

} // namespace alg
