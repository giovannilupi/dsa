#pragma once

#include <queue>
#include <unordered_map>
#include <vector>
#include "binary_tree.hpp"

namespace alg {

/**
 * Callback function to be applied to each node of a graph.
 */
using GCallback = std::function<void(int&)>;

namespace detail {

/**
 * Helper function for recursively reconstructing the shortest path from destination to source vertex.
 * We leverage the call stack to reverse the path, obtaining a path from source to destination.
 */
inline bool extractMinPathRecHelper(const std::vector<int>& parent, int source, int dest, std::vector<int>& path) {
    if (source == dest) {
        path.push_back(source);
        return true;
    }
    // The current node has no parents, so it's unreachable
    if (parent[dest] == -1) return false;
    // Recurse and propagate failure if occurs
    if (!extractMinPathRecHelper(parent, source, parent[dest], path)) return false;
    // No failure, add current node to path
    path.push_back(dest);
    return true;
}

/**
 * Helper function to perform DFS with an optional callback in preorder or postorder.
 */
inline void dfsGraphApplyHelper(const std::vector<std::vector<int>>& adjList, int source, std::vector<bool>& visited, alg::GCallback callback, bool postorder = false) {
    visited[source] = true;
    if (!postorder) callback(source);
    for (int neighbor : adjList[source]) {
        if (!visited[neighbor]) {
            dfsGraphApplyHelper(adjList, neighbor, visited, callback, postorder);
        }
    }
    if (postorder) callback(source);   
}

/**
 * Helper function to convert a binary tree to an undirected graph.
 */
inline void binTreeToGraphHelper(const TreeNode<int>* root, std::unordered_map<int, std::vector<int>>& adjList) {
    if (!root) return;
    // Ensure the current node is added to the adjacency list, even if it has no children
    adjList[root->val];
    // Process left subtree
    if (root->left) {
        // Connect current node to its left child and vice versa
        adjList[root->val].push_back(root->left->val);
        adjList[root->left->val].push_back(root->val);
        binTreeToGraphHelper(root->left, adjList);
    }
    // Process right subtree
    if (root->right) {
        // Connect current node to its right child and vice versa
        adjList[root->val].push_back(root->right->val);
        adjList[root->right->val].push_back(root->val);
        binTreeToGraphHelper(root->right, adjList);
    }
}

/**
 * Helper function for checking if an undirected graph is cyclic.
 * It checks if there is an edge connecting a vertex to any node that has already been visited and is not its parent.
 */
inline bool isCyclicUndirectedHelper(const std::vector<std::vector<int>>& adjList, int source, std::vector<bool>& visited, int parent) {
    visited[source] = true;
    for (int neighbor : adjList[source]) {
        if (!visited[neighbor]) {
            if (isCyclicUndirectedHelper(adjList, neighbor, visited, source)) return true;
        }
        // If neighbor is visited and is not the parent, we found a cycle
        else if (neighbor != parent) return true;
    }
    return false;
}

/**
 * Helper function for checking if an directed graph is cyclic.
 * It checks if there is an edge connecting a vertex to any ancestor that has already been visited.
 */
inline bool isCyclicDirectedHelper(const std::vector<std::vector<int>>& adjList, int source, std::vector<bool>& visited, std::vector<bool>& recStack) {
    visited[source] = true;
    recStack[source] = true;
    for (int neighbor : adjList[source]) {
        if (!visited[neighbor]) {
            if (isCyclicDirectedHelper(adjList, neighbor, visited, recStack)) return true;
        }
        else if (recStack[neighbor]) return true;
    }
    recStack[source] = false;
    return false;
}

/**
 * Helper function for topological sort.
 * This algorithm leverages the DFS timestamping property to traverse the graph in reverse topological order.
 * The result is stored in a vector.
 * The caller obtains the result by reversing the result vector.
 */
inline void topologicalSortHelper(const std::vector<std::vector<int>>& adjList, int source, std::vector<bool>& visited, std::vector<int>& finished) {
    visited[source] = true;
    for (int neighbor : adjList[source]) {
        if (!visited[neighbor]) {
            topologicalSortHelper(adjList, neighbor, visited, finished);
        }
    }
    finished.push_back(source);
}

} // namespace detail

/**
 * Applies a function to each vertex of a graph in BFS level-order traversal.
 * The graph is represented as an adjacency list.
 */
inline void bfsGraphApply(const std::vector<std::vector<int>>& adjList, int source, GCallback callback) {
    if (adjList.empty() || source < 0 || source >= adjList.size()) return;
    // Tracks visited nodes
    std::vector<bool> visited(adjList.size(), false);
    std::queue<int> q;
    // Start from the source vertex
    q.push(source);
    visited[source] = true;
    while (!q.empty()) {
        int curr = q.front();
        q.pop();
        // Apply function
        callback(curr);
        // Traverse all the adjacent vertices of the current vertex
        for (int neighbor : adjList[curr]) {
            if (!visited[neighbor]) {
                q.push(neighbor);
                // Mark the neighbor visited to avoid adding it to the queue again
                visited[neighbor] = true;
            }
        }
    }
}

/**
 * Returns the breadth-first tree built by the BFS procedure.
 * This tree contains the shortest path from the source vertex to all other reachable vertices.
 */
inline std::vector<int> getBfsTree(const std::vector<std::vector<int>>& adjList, int source) {
    if (adjList.empty() || source < 0 || source >= adjList.size()) return {};
    // Tracks visited nodes
    std::vector<bool> visited(adjList.size(), false);
    std::vector<int> parent(adjList.size(), -1);
    std::queue<int> q;
    // Start from the source vertex
    q.push(source);
    visited[source] = true;
    while (!q.empty()) {
        int curr = q.front();
        q.pop();
        // Traverse all the adjacent vertices of the current vertex
        for (int neighbor : adjList[curr]) {
            if (!visited[neighbor]) {
                q.push(neighbor);
                // Mark the neighbor visited to avoid adding it to the queue again
                visited[neighbor] = true;
                parent[neighbor] = curr;
            }
        }
    }
    return parent;
}

/**
 * Returns the shortest path from the source vertex to the destination vertex
 * using the BFS tree built by the BFS procedure.
 * This uses a recursive implementation.
 * The source node must be equal to the root of the BFS tree, behavior is undefined otherwise.
 * Returns an empty vector if the destination vertex is unreachable from the source of the BFS tree.
 */
inline std::vector<int> extractMinPathRec(const std::vector<int>& parent, int source, int dest) {
    if (parent.empty() || dest < 0 || dest >= parent.size() || source < 0 || source >= parent.size()) return {};
    std::vector<int> path;
    if (!detail::extractMinPathRecHelper(parent, source, dest, path)) return {};
    return path;
}

/**
 * Returns the shortest path from the source vertex to the destination vertex
 * using the BFS tree built by the BFS procedure.
 * This uses an iterative implementation.
 * The source node must be equal to the root of the BFS tree, behavior is undefined otherwise.
 * Returns an empty vector if the destination vertex is unreachable from the source of the BFS tree.
 */
inline std::vector<int> extractMinPathIter(const std::vector<int>& parent, int source, int dest) {
    if (parent.empty() || dest < 0 || dest >= parent.size() || source < 0 || source >= parent.size()) return {};
    std::vector<int> path;
    for (int curr = dest; curr != -1; curr = parent[curr]) path.push_back(curr);
    if (path.back() != source) return {};
    std::reverse(path.begin(), path.end());
    return path;
}

/**
 * Returns the shortest path length from the source vertex to the destination vertex.
 * Uses BFS to find the shortest path in an unweighted graph.
 * Returns -1 if the destination is not reachable from the source.
 */
inline int shortestPathLen(const std::vector<std::vector<int>>& adjList, int source, int destination) {
    if (adjList.empty() || source < 0 || source >= adjList.size() || destination < 0 || destination >= adjList.size()) {
        return -1;
    }
    std::vector<bool> visited(adjList.size(), false);
    std::queue<int> q;
    q.push(source);
    visited[source] = true;
    int dist = 0;
    while (!q.empty()) {
        int qsz = q.size();
        for (int i = 0; i != qsz; ++i) {
            int curr = q.front();
            q.pop();
            if (curr == destination) return dist;
            for (int neighbor : adjList[curr]) {
                if (!visited[neighbor]) {
                    q.push(neighbor);
                    visited[neighbor] = true;
                }
            }
        }
        ++dist;
    }
    return -1;
}

/**
 * Applies a function to each node of a graph in DFS pre-order traversal.
 */
inline void preorderGraphApply(const std::vector<std::vector<int>>& adjList, int source, GCallback callback) {
    if (adjList.empty() || source < 0 || source >= adjList.size()) return;
    std::vector<bool> visited(adjList.size(), false);
    detail::dfsGraphApplyHelper(adjList, source, visited, callback, false);
}

/**
 * Applies a function to each node of a graph in DFS post-order traversal.
 */
inline void postorderGraphApply(const std::vector<std::vector<int>>& adjList, int source, GCallback callback) {
    if (adjList.empty() || source < 0 || source >= adjList.size()) return;
    std::vector<bool> visited(adjList.size(), false);
    detail::dfsGraphApplyHelper(adjList, source, visited, callback, true);
}

/**
 * Checks if an undirected graph has a cycle.
 * Complexity: O(|V| + |E|)
 */
inline bool isCyclicUndirected(const std::vector<std::vector<int>>& adjList) {
    if (adjList.empty()) return false;
    std::vector<bool> visited(adjList.size(), false);
    for (int i = 0; i != adjList.size(); ++i) {
        if (!visited[i]) {
            if (detail::isCyclicUndirectedHelper(adjList, i, visited, -1)) return true;
        }
    }
    return false;
}

/**
 * Checks if a directed graph has a cycle.
 * Complexity: O(|V| + |E|)
 */
inline bool isCyclicDirected(const std::vector<std::vector<int>>& adjList) {
    if (adjList.empty()) return false;
    std::vector<bool> visited(adjList.size(), false);
    std::vector<bool> recStack(adjList.size(), false);
    for (int i = 0; i != adjList.size(); ++i) {
        if (!visited[i]) {
            if (detail::isCyclicDirectedHelper(adjList, i, visited, recStack)) return true;
        }
    }
    return false;
}

/**
 * Converts a binary tree to its equivalent undirected graph.
 * The graph is represented as an adjacency list.
 * The value of each node is used as vertex number in the graph.
 * The adjacency list is represented as a hash map, since the tree might contain any value.
 * The tree must not contain duplicates, behavior is undefined if this is the case.
 * Complexity: O(n)
 */
inline std::unordered_map<int, std::vector<int>> binTreeToGraph(const TreeNode<int>* root) {
    std::unordered_map<int, std::vector<int>> adjList;
    detail::binTreeToGraphHelper(root, adjList);
    return adjList;
}

/**
 * Returns the topological order of a directed acyclic graph.
 * The graph is represented as an adjacency list.
 * Note that one graph can have multiple valid topological orders.
 * If the graph is cyclic, the behavior is undefined.
 * If the graph is not guaranteed to be acyclic, 
 * this function can be augmented with the cycle detection algorithm.
 * Complexity: O(|V| + |E|)
 */
inline std::vector<int> topologicalSort(const std::vector<std::vector<int>>& adjList) {
    if (adjList.empty()) return {};
    std::vector<bool> visited(adjList.size(), false);
    std::vector<int> finished;
    for (int i = 0; i != adjList.size(); ++i) {
        if (!visited[i]) {
            detail::topologicalSortHelper(adjList, i, visited, finished);
        }
    }
    std::reverse(finished.begin(), finished.end());
    return finished;
}

/**
 * Returns the center of a tree (a connected acyclic undirected graph).
 * The center of a graph is the set of all vertices of minimum eccentricity.
 * The center can be made of one or two vertices, depending on the length of the tree diameter.
 * Center nodes are ideal for rooting the tree.
 * Complexity: O(|V| + |E|)
 */
inline std::vector<int> getTreeCenter(const std::vector<std::vector<int>>& adjList) {
    // Check input
    auto sz = adjList.size();
    if (!sz) return {};
    if (sz == 1) return {0};
    // Find the degree of each vertex
    std::vector<int> degree(sz, 0);
    for (int i = 0; i != sz; ++i) {
        degree[i] = adjList[i].size();
    }
    // Identify leaves
    std::queue<int> leaves;
    for (int i = 0; i != sz; ++i) {
        if (degree[i] == 1) leaves.push(i);
    }
    // Find center
    while (sz > 2) {
        int leavesSize = leaves.size();
        sz -= leavesSize;
        // Process leaves one layer at a time
        for (int i = 0; i != leavesSize; ++i) {
            int leaf = leaves.front();
            leaves.pop();
            for (int neighbor : adjList[leaf]) {
                if (--degree[neighbor] == 1) leaves.push(neighbor);
            }
        }
    }
    // Copy the queue to a vector
    std::vector<int> center;
    while (!leaves.empty()) {
        center.emplace_back(leaves.front());
        leaves.pop();
    }
    return center;
}

} // namespace alg
