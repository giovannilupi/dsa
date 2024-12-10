#pragma once

#include <format>
#include <numeric>
#include <vector>

namespace alg {

/**
 * Implementation of the union find (disjoint set) data structure.
 * Notice this is a static (as opposed to dynamic) implementation.
 * This means that the make-set operation is implicitly handled in the constructor.
 * Once the union find object is created, it is not possible to add new nodes.
 */
class UnionFind {
    /** Stores the parent of each node. */
    std::vector<int> parent;
    /** Stores the rank of each node. */
    std::vector<int> rank;
public:
    /**
     * Constructs a union find object with n nodes (0 to n - 1).
     */
    UnionFind(int n) : parent(n), rank(n, 0) {
        // Initialize the parent of each node to itself
        std::iota(parent.begin(), parent.end(), 0);
    }

    /**
     * Finds the root of the set containing x (representative of the set).
     * This implementation uses path compression to optimize subsequent operations.
     */
    int find(int x) {
        if (x < 0 || x >= parent.size()) throw std::out_of_range(std::format("Node {} is out of range", x));
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }

    /**
     * Unites the sets containing x and y.
     * This implementation uses union by rank to minimize the height of the tree.
     */
    void unite(int x, int y) {
        if (x < 0 || x >= parent.size()) throw std::out_of_range(std::format("Node {} is out of range", x));
        if (y < 0 || y >= parent.size()) throw std::out_of_range(std::format("Node {} is out of range", y));
        int xRoot = find(x);
        int yRoot = find(y);
        // Already in the same set
        if (xRoot == yRoot) return;
        // Attach the smaller tree to the larger tree
        if (rank[xRoot] < rank[yRoot]) parent[xRoot] = yRoot; 
        else if (rank[xRoot] > rank[yRoot]) parent[yRoot] = xRoot;
        // If they have the same rank, arbitrarily choose a parent
        // The resulting tree has one additional layer, so we increment its rank
        else { parent[xRoot] = yRoot; ++rank[yRoot]; }
    }
};

} // namespace alg
