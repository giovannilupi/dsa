#include <gtest/gtest.h>
#include "graph_weighted.hpp"

using WeightedGraph = std::vector<std::vector<std::pair<int, int>>>;

TEST(GraphTest, MSTWork) {
    // Graph with distinct weights, hence a unique MST
    WeightedGraph graph = {
        {{1, 1}, {2, 2}},   // Node 0: edges to 1 (weight 1), 2 (weight 2)
        {{0, 1}, {2, 3}},   // Node 1: edges to 0 (weight 1), 2 (weight 3)
        {{0, 2}, {1, 3}}    // Node 2: edges to 0 (weight 2), 1 (weight 3)
    };
    // Expected MST: (0-1, weight 1), (0-2, weight 2) -> Total weight = 3
    EXPECT_EQ(alg::mstKruskal(graph), 3);
    EXPECT_EQ(alg::mstPrim(graph), 3);
    
    // Graph with duplicate weights, hence multiple MSTs can exist
    graph = {
        {{1, 1}, {2, 2}, {3, 3}},  // Node 0: edges to 1 (weight 1), 2 (weight 2), 3 (weight 3)
        {{0, 1}, {3, 2}},              // Node 1: edges to 0 (weight 1), 3 (weight 2)
        {{0, 2}, {3, 2}},              // Node 2: edges to 0 (weight 2), 3 (weight 2)
        {{0, 3}, {1, 2}, {2, 2}}   // Node 3: edges to 0 (weight 3), 1 (weight 2), 2 (weight 2)
    };
    // MST1: (0-1, weight 1), (1-3, weight 2), (2-3, weight 2) -> Total weight = 5
    // MST2: (0-1, weight 1), (1-3, weight 2), (0-2, weight 2) -> Total weight = 5
    EXPECT_EQ(alg::mstKruskal(graph), 5);
    EXPECT_EQ(alg::mstPrim(graph), 5);
}

TEST(GraphTest, DjikstraWorks) {
    // Graph with single node and no edges
    WeightedGraph graph = {
        {}   // Node 0 has no edges
    };
    std::vector<int> expected = {0};
    ASSERT_EQ(alg::djikstra(graph, 0), expected);

    // Graph with unreachable node
    graph = {
        {{1, 2}},    // Node 0: edges to 1 (weight 2)
        {{0, 2}},    // Node 1: edges to 0 (weight 2)
        {{}}         // Node 2: disconnected
    };
    
    expected = {0, 2, std::numeric_limits<int>::max()};
    ASSERT_EQ(alg::djikstra(graph, 0), expected);

    // Connected graph
    graph = {
        {{1, 4}, {2, 1}},              // Node 0: edges to 1 (weight 4), 2 (weight 1)
        {{0, 4}, {2, 2}, {3, 1}},  // Node 1: edges to 0 (weight 4), 2 (weight 2), 3 (weight 1)
        {{0, 1}, {1, 2}, {3, 5}},  // Node 2: edges to 0 (weight 1), 1 (weight 2), 3 (weight 5)
        {{1, 1}, {2, 5}}               // Node 3: edges to 1 (weight 1), 2 (weight 5)
    };
    expected = {0, 3, 1, 4};
    ASSERT_EQ(alg::djikstra(graph, 0), expected);
}
