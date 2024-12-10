#pragma once

#include <algorithm>
#include <limits>
#include <queue>
#include <tuple>
#include <vector>
#include "union_find.hpp"

namespace alg {

/**
 * Computes the minimum spanning tree weight of the given graph using Kruskal's algorithm.
 * The algorithm applies to unidrected, connected, weighted graphs.
 * If the graph is not connected, the behavior is undefined.
 */
inline int mstKruskal(const std::vector<std::vector<std::pair<int, int>>>& adjList) {
    int n = adjList.size();
    // Collect edges
    std::vector<std::tuple<int, int, int>> edges;
    for (int u = 0; u != n; ++u) {
        for (const auto &[v, weight] : adjList[u]) {
            // Avoid duplicate edges, given the graph is undirected
            if (u < v) edges.emplace_back(weight, u, v);
        }
    }
    // Sort edges
    std::sort(edges.begin(), edges.end());
    // Setup
    alg::UnionFind uf(n);
    int mstWeight = 0;
    int mstEdges = 0;
    // Process edges in order
    for (const auto &[weight, u, v] : edges) {
        if (uf.find(u) != uf.find(v)) {
            uf.unite(u, v);
            mstWeight += weight;
            if (++mstEdges == n - 1) break;
        }
    }
    return mstWeight;
}

/**
 * Computes the minimum spanning tree weight of the given graph using Prim's algorithm.
 * The algorithm applies to unidrected, connected, weighted graphs.
 * If the graph is not connected, the behavior is undefined.
 */
inline int mstPrim(const std::vector<std::vector<std::pair<int, int>>>& adjList) {
    int n = adjList.size();
    // Tracks nodes included in the MST
    std::vector<bool> visited(n, false);
    // Stores minimum weight edge connecting each node to the MST
    std::vector<int> dist(n, std::numeric_limits<int>::max());
    // Efficiently extract the minimum weight edge
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> pq;
    // Start algorithm from source node 0
    pq.emplace(0, 0); 
    dist[0] = 0;
    int mstWeight = 0;
    while (!pq.empty()) {
        const auto [w, u] = pq.top();
        pq.pop();
        // Outdated edge
        if (visited[u]) continue;
        visited[u] = true;
        mstWeight += w;
        // Add each outgoing edge to the priority queue
        for (const auto &[v, weight] : adjList[u]) {
            // Do not add outdated edges 
            if (!visited[v] && dist[v] > weight) {
                dist[v] = weight;
                pq.emplace(weight, v);
            }
        }
    }
    return mstWeight;
}

/**
 * Computes the shortest path from the source node to all other nodes using Dijkstra's algorithm.
 * The algorithm applies to directed, weighted graphs with positive edge weights.
 * The result is returned as a vector of shortest distances to each node.
 * If a node is unreachable from the source, the distance is set to std::numeric_limits<int>::max().
 */
inline std::vector<int> djikstra(const std::vector<std::vector<std::pair<int, int>>>& adjList, int source) {
    int n = adjList.size();
    // Stores shortest distance to each node from the source
    std::vector<int> dist(n, std::numeric_limits<int>::max());
    dist[source] = 0;
    // Min-heap setup
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> pq;
    pq.emplace(0, source);
    while (!pq.empty()) {
        const auto [d, u] = pq.top();
        pq.pop();
        if (d > dist[u]) continue;
        for (const auto &[v, weight] : adjList[u]) {
            // Relax edge (u, v)
            if (d + weight < dist[v]) {
                dist[v] = d + weight;
                pq.emplace(dist[v], v);
            }
        }
    }
    return dist;    
}

} // namespace alg
