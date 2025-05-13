#pragma once

#include "alg_concepts.hpp"
#include <algorithm>
#include <cstddef>
#include <functional>
#include <queue>
#include <concepts>
#include "common.hpp"

namespace alg {

/*
 * Node of a binary tree.
 * For built-in types, the `val` field must be initialized explicitly at construction.
 */
template <typename T>
struct TreeNode {
    T val;
    TreeNode* left = nullptr;
    TreeNode* right = nullptr;
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

/**
 * The reference to diameter is similar to using a global variable.
 * We store in the variable the maximum diameter found so far.
 * Aside from that, this is a recursive function that returns the height of the tree.
 */
template <typename T>
std::size_t getTreeDiameterHelper(const TreeNode<T>* root, std::size_t& diameter) {
    if (!root) return 0;
    // Find the height of the left and right subtrees
    std::size_t lHeight = getTreeDiameterHelper(root->left, diameter);
    std::size_t rHeight = getTreeDiameterHelper(root->right, diameter);
    // The diameter may or may not pass from the root
    diameter = std::max(diameter, lHeight + rHeight + 1);
    // Typical pattern for returning the height
    return std::max(lHeight, rHeight) + 1;
}

/**
 * Helper function for checking if the tree is height-balanced.
 */
template <typename T>
int isHeightBalancedHelper(const TreeNode<T>* root, bool& balanced) {
    if (!root || !balanced) return 0;
    int lHeight = isHeightBalancedHelper(root->left, balanced);
    int rHeight = isHeightBalancedHelper(root->right, balanced);
    if (std::abs(lHeight - rHeight) > 1) balanced = false;
    return std::max(lHeight, rHeight) + 1;
}

} // namespace detail

/**
 * Callback function to be applied to each node of a tree.
 */
template <typename T>
using TCallback = std::function<void(T&)>;

/**
 * Applies a function to each node of a tree in DFS pre-order traversal.
 */
template <typename T>
void preorderTreeApply(TreeNode<T>* root, TCallback<T> func) {
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
void inorderTreeApply(TreeNode<T>* root, TCallback<T> func) {
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
void postorderTreeApply(TreeNode<T>* root, TCallback<T> func) {
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
void bfsTreeApply(TreeNode<T>* root, TCallback<T> func) {
    if (!root) return;
    std::queue<TreeNode<T>*> child_queue;
    child_queue.push(root);
    while (!child_queue.empty()) {
        TreeNode<T>* node = child_queue.front();
        child_queue.pop();
        func(node->val);
        if (node->left) child_queue.push(node->left);
        if (node->right) child_queue.push(node->right);
    }    
}

/**
 * Converts a binary tree into a vector.
 * The vector contains the values in level-order.
 */
template <typename T>
std::vector<T> toVector(TreeNode<T>* root) {
    std::vector<T> vec;
    bfsTreeApply<T>(root, [&vec](T& val) { vec.push_back(val); });
    return vec;
}

/**
 * Converts an iterator range into a binary tree.
 * The tree is created in level-order.
 */
template <std::input_or_output_iterator Iter>
TreeNode<typename std::iterator_traits<Iter>::value_type>* toBinaryTree(Iter first, Iter last) {
    using V = typename std::iterator_traits<Iter>::value_type;
    if (first == last) return nullptr;
    TreeNode<V>* root = new TreeNode<V>{.val = *first++};
    std::queue<TreeNode<V>*> child_queue;
    child_queue.push(root);
    Iter it = first;
    while (!child_queue.empty() && it != last) {
        TreeNode<V>* node = child_queue.front();
        child_queue.pop();
        if (it != last) {
            node->left = new TreeNode<V>{.val = *it++};
            child_queue.push(node->left);
        }
        if (it != last) {
            node->right = new TreeNode<V>{.val = *it++};
            child_queue.push(node->right);
        }
    }
    return root;
}

/**
 * Converts a container into a binary tree.
 * The tree is created in level-order.
 */
template <std::ranges::range T>
TreeNode<typename T::value_type>* toBinaryTree(const T& container) {
    return toBinaryTree(container.begin(), container.end());
}
/**
 * Converts an initializer list into a binary tree.
 * The tree is created in level-order.
 */
template <typename T>
TreeNode<T>* toBinaryTree(std::initializer_list<T> ilist) {
    return toBinaryTree(ilist.begin(), ilist.end());
}

/**
 * Finds the lowest common ancestor of two nodes in a binary tree.
 * This algorithms uses a single pre-order traversal.
 * This assumes both nodes supplied exist in the tree.
 * If the none of the nodes exist in the tree, returns nullptr.
 * If only one of the nodes exists in the tree, returns that node.
 */
template <typename T>
const TreeNode<T>* lcaTree(const TreeNode<T>* root, const TreeNode<T>* n1, const TreeNode<T>* n2) {
    // If the root matches one of the nodes, it is the LCA
    if (!root || root == n1 || root == n2) return root;
    const TreeNode<T>* left = lcaTree(root->left, n1, n2);
    const TreeNode<T>* right = lcaTree(root->right, n1, n2);
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
    return new TreeNode<T>{.val = root->val, .left = left, .right = right};
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
 * Checks if a binary tree is height balanced.
 * The depth of the two subtrees of every node should never differ by more than one.
 */
template <typename T>
bool isHeightBalanced(const TreeNode<T>* root) {
    bool balanced = true;
    detail::isHeightBalancedHelper(root, balanced);
    return balanced;
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
 * The height of an empty tree is 0, and the height of a tree with one node is 1.
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

/**
 * Returns the level of a key in a binary tree.
 * If there is no such key, by default returns -1.
 * By changing the default value, you can start the count from a different number (e.g. 1).
 * If there are multiple keys with the same value, returns the level of the first key found.
 */
template <typename T>
index getTreeNodeLevel(const TreeNode<T>* root, const T& val, std::size_t level = 0) {
    if (!root) return -1;
    if (root->val == val) return level;
    index left = getTreeNodeLevel(root->left, val, level+1);
    if (left != -1) return left;
    return getTreeNodeLevel(root->right, val, level+1);
}

/**
 * Mirrors a binary tree.
 */
template <typename T>
void mirrorTree(TreeNode<T>* root) {
    if (!root) return;
    std::swap(root->left, root->right);
    mirrorTree(root->left);
    mirrorTree(root->right);
}

/**
 * Checks if two binary trees are mirror trees.
 */
template <typename T>
bool checkMirrorTree(const TreeNode<T>* root1, const TreeNode<T>* root2) {
    if (!root1 && !root2) return true;
    if (!root1 || !root2) return false;
    if (root1->val != root2->val) return false;
    return checkMirrorTree(root1->left, root2->right) &&
           checkMirrorTree(root1->right, root2->left);
}

/**
 * Returns the diameter of a binary tree.
 * The diameter of a tree is the number of nodes on the longest path between two leaf nodes.
 */
template <typename T>
std::size_t getTreeDiameter(const TreeNode<T>* root) {
    std::size_t diameter = 0;
    detail::getTreeDiameterHelper(root, diameter);
    return diameter;
}

} // namespace alg
