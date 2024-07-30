#pragma once

#include "alg_concepts.hpp"
#include <functional>
#include <queue>
#include <concepts>

namespace alg {

/*
 * Node of a binary tree.
 */
template <typename T>
struct TreeNode {
    T val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(const T& val) : val(val) {}
    TreeNode(const T& val, TreeNode* left, TreeNode* right) : val(val), left(left), right(right) {}
};

namespace detail {

/**
 * Finds the node in the tree with the extreme value according to the given comparator.
 */
template <typename T, typename Cmp>
    requires std::predicate<Cmp, T, T>
const TreeNode<T>* findExtremeTreeNode(const TreeNode<T>* root, Cmp cmp) {
    if (!root) return nullptr;
    const TreeNode<T>* left = findExtremeTreeNode(root->left, cmp);
    const TreeNode<T>* right = findExtremeTreeNode(root->right, cmp);
    const TreeNode<T>* extreme = root;
    if (left && cmp(left->val, extreme->val)) extreme = left;
    if (right && cmp(right->val, extreme->val)) extreme = right;
    return extreme;
}

}

/**
 * Callback function to be applied to each node of a tree.
 */
template <typename T>
using Callback = std::function<void(const T&)>;

/**
 * Applies a function to each node of a tree in DFS pre-order traversal.
 */
template <typename T>
void preorderTreeApply(const TreeNode<T>* root, Callback<T> func) {
    if (root) {
        func(root->val);
        preorderTreeApply(root->left, func);
        preorderTreeApply(root->right, func);
    }
}

/**
 * Applies a function to each node of a tree in DFS in-order traversal.
 */
template <typename T>
void inorderTreeApply(const TreeNode<T>* root, Callback<T> func) {
    if (root) {
        inorderTreeApply(root->left, func);
        func(root->val);
        inorderTreeApply(root->right, func);
    }
}

/**
 * Applies a function to each node of a tree in DFS post-order traversal.
 */
template <typename T>
void postorderTreeApply(const TreeNode<T>* root, Callback<T> func) {
    if (root) {
        postorderTreeApply(root->left, func);
        postorderTreeApply(root->right, func);
        func(root->val);
    }
}

/**
 * Applies a function to each node of a tree in BFS level-order traversal.
 */
template <typename T>
void bfsTreeApply(const TreeNode<T>* root, Callback<T> func) {
    if (!root) return;
    std::queue<const TreeNode<T>*> child_queue;
    child_queue.push(root);
    while (!child_queue.empty()) {
        const TreeNode<T>* node = child_queue.front();
        func(node->val);
        child_queue.pop();
        if (node->left) child_queue.push(node->left);
        if (node->right) child_queue.push(node->right);
    }    
}

/**
 * Finds the lowest common ancestor of two nodes in a binary tree.
 * This algorithms uses a single pre-order traversal.
 * This assumes both nodes supplied exist in the tree.
 * If the none of the nodes exist in the tree, returns nullptr.
 * If only one of the nodes exists in the tree, returns that node.
 */
template <typename T>
TreeNode<T>* lcaTree(const TreeNode<T>* root, const TreeNode<T>* n1, const TreeNode<T>* n2) {
    // If the root matches one of the nodes, it is the LCA
    if (!root || root == n1 || root == n2) return root;
    TreeNode<T>* left = lcaTree(root->left, n1, n2);
    TreeNode<T>* right = lcaTree(root->right, n1, n2);
    // If both children match one of the nodes, then the current node is the LCA
    if (left && right) return root;
    // The LCA is the node that matches one of the input nodes
    return left ? left : right;
}

/**
 * Deep copies a binary tree into a new tree.
 * This algorithm uses a single post-order traversal.
 */
template <typename T>
TreeNode<T>* copyTree(const TreeNode<T>* root) {
    if (!root) return nullptr;
    TreeNode<T>* left = copyTree(root->left);
    TreeNode<T>* right = copyTree(root->right);
    return new TreeNode<T>(root->val, left, right);
}

/**
 * Deletes a binary tree.
 * This algorithm uses a single post-order traversal.
 */
template <typename T>
void deleteTree(const TreeNode<T>* root) {
    if (root) {
        deleteTree(root->left);
        deleteTree(root->right);
        delete(root);
    }
}

/**
 * Checks if two binary trees are equal.
 * This use pre-order traversal.
 */
template <typename T>
bool checkEqualTrees(const TreeNode<T>* root1, const TreeNode<T>* root2) {
    if (!root1 && !root2) return true;
    // If only one of the trees is empty, they are not equal
    if (!root1 || !root2) return false;
    if (root1->val != root2->val) return false;
    return checkEqualTrees(root1->left, root2->left) &&
           checkEqualTrees(root1->right, root2->right);
}

/**
 * Finds a node with a given value in a binary tree.
 */
template <typename T>
const TreeNode<T>* findTreeNode(const TreeNode<T>* root, const T& val) {
    if (!root) return nullptr;
    if (root->val == val) return root;
    const TreeNode<T>* left = findTreeNode(root->left, val);
    if (left) return left;
    return findTreeNode(root->right, val);
}

/**
 * Returns the node with the minimum value in a binary tree.
 */
template <typename T>
    requires std::strict_weak_order<std::less<T>, T, T>
const TreeNode<T>* findMinTreeNode(const TreeNode<T>* root) {
    return detail::findExtremeTreeNode(root, std::less<T>());
}

/**
 * Returns the node with the maximum value in a binary tree.
 */
template <typename T>
    requires std::strict_weak_order<std::greater<T>, T, T>
const TreeNode<T>* findMaxTreeNode(const TreeNode<T>* root) {
    return detail::findExtremeTreeNode(root, std::greater<T>());
}

/**
 * Returns the number of nodes in a binary tree.
 * This algorithm uses post-order traversal.
 */
template <typename T>
std::size_t getTreeSize(const TreeNode<T>* root) {
    if (!root) return 0;
    std::size_t left = getTreeSize(root->left);
    std::size_t right = getTreeSize(root->right);
    return 1 + left + right;
}

/**
 * Returns the height of a binary tree.
 * Here we define the height as the number of nodes in the longest path from the root to a leaf.
 * The height of an empty tree with is 0, and the height of a tree with one node is 1.
 */
template <typename T>
std::size_t getTreeHeight(const TreeNode<T>* root) {
    if (!root) return 0;
    std::size_t left = getTreeHeight(root->left);
    std::size_t right = getTreeHeight(root->right);
    return std::max(left, right) + 1;
}

/**
 * Returns the sum of all values in a binary tree.
 * This algorithm uses post-order traversal.
 */
template <std::integral T>
T accumulateTree(const TreeNode<T>* root) {
    if (!root) return 0;
    T left = accumulateTree(root->left);
    T right = accumulateTree(root->right);
    return root->val + left + right;
}

} // namespace alg
