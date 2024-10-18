#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <unordered_set>
#include "graph_unweighted.hpp"

using namespace alg;

static const std::map<std::string, std::vector<std::vector<int>>> directedGraphs = {
{
        "DirectedSimpleGraph", 
        {
            {1, 2},   // Node 0 is connected to nodes 1 and 2
            {},               // Node 1 has no outgoing edges
            {}                // Node 2 has no outgoing edges
        }
    },
{
        "DirectedLinearGraph", 
        {
            {1},      // Node 0 is connected to node 1
            {2},      // Node 1 is connected to node 2
            {3},      // Node 2 is connected to node 3
            {}            // Node 3 has no outgoing edges
        }
    },
{
        "DirectedCycleGraph", 
        {
            {1},      // Node 0 is connected to node 1
            {2},      // Node 1 is connected to node 2
            {0},      // Node 2 is connected back to node 0
        }
    },
{
        "DirectedDisconnectedGraph", 
        {
            {1},      // Node 0 is connected to node 1
            {},           // Node 1 has no outgoing edges
            {3},      // Node 2 is connected to node 3
            {}            // Node 3 has no outgoing edges
        }
    },
{
        "DirectedStarGraph", 
        {
            {1, 2, 3},  // Node 0 is connected to nodes 1, 2, and 3
            {},                     // Node 1 has no outgoing edges
            {},                     // Node 2 has no outgoing edges
            {}                      // Node 3 has no outgoing edges
        }
    },
{
        "DirectedBinaryTree", 
        {
            {1, 2},   // Node 0 is connected to nodes 1 and 2
            {3, 4},   // Node 1 is connected to nodes 3 and 4
            {},               // Node 2 has no outgoing edges
            {},               // Node 3 has no outgoing edges
            {}                // Node 4 has no outgoing edges
        }
    },
{
        "DirectedCompleteGraph", 
        {
            {1, 2, 3}, // Node 0 is connected to nodes 1, 2, and 3
            {0, 2, 3}, // Node 1 is connected to nodes 0, 2, and 3
            {0, 1, 3}, // Node 2 is connected to nodes 0, 1, and 3
            {0, 1, 2}  // Node 3 is connected to nodes 0, 1, and 2
        }
    },
{
        "DirectedRandomGraph", 
        {
            {1, 3},         // Node 0 is connected to nodes 1 and 3
            {0, 2},         // Node 1 is connected to nodes 0 and 2
            {1, 3, 4},  // Node 2 is connected to nodes 1, 3, and 4
            {0, 2},         // Node 3 is connected to nodes 0 and 2
            {}                      // Node 4 has no outgoing edges
        }
    },
{
        "DirectedTreeStructure", 
        {
            {1, 2},    // Node 0 is connected to nodes 1 and 2
            {3, 4},    // Node 1 is connected to nodes 3 and 4
            {5},           // Node 2 is connected to node 5
            {},                // Node 3 has no outgoing edges
            {},                // Node 4 has no outgoing edges
            {}                 // Node 5 has no outgoing edges
        }
    }
};

static const std::map<std::string, std::vector<std::vector<int>>> undirectedGraphs = {
{
        "UndirectedSimpleGraph",
        {
            {1, 2},   // Node 0 is connected to nodes 1 and 2
            {0},          // Node 1 is connected to node 0
            {0}           // Node 2 is connected to node 0
        }
    },
{
        "UndirectedLinearGraph",
        {
            {1},          // Node 0 is connected to node 1
            {0, 2},   // Node 1 is connected to nodes 0 and 2
            {1, 3},   // Node 2 is connected to nodes 1 and 3
            {2}           // Node 3 is connected to node 2
        }
    },
{
        "UndirectedCycleGraph",
        {
            {1, 2},   // Node 0 is connected to nodes 1 and 2
            {0, 2},   // Node 1 is connected to nodes 0 and 2
            {0, 1}    // Node 2 is connected to nodes 0 and 1 (forms a cycle)
        }
    },
{
        "UndirectedDisconnectedGraph",
        {
            {1},      // Node 0 is connected to node 1
            {0},      // Node 1 is connected to node 0
            {3},      // Node 2 is connected to node 3
            {2}       // Node 3 is connected to node 2
        }
    },
{
        "UndirectedStarGraph",
        {
            {1, 2, 3},  // Node 0 is connected to nodes 1, 2, and 3
            {0},                // Node 1 is connected to node 0
            {0},                // Node 2 is connected to node 0
            {0}                 // Node 3 is connected to node 0
        }
    },
{
        "UndirectedCompleteGraph",
        {
            {1, 2, 3},  // Node 0 is connected to nodes 1, 2, and 3
            {0, 2, 3},  // Node 1 is connected to nodes 0, 2, and 3
            {0, 1, 3},  // Node 2 is connected to nodes 0, 1, and 3
            {0, 1, 2}   // Node 3 is connected to nodes 0, 1, and 2
        }
    },
{
        "UndirectedTreeGraph",
        {
            {1, 2},         // Node 0 is connected to nodes 1 and 2
            {0, 3, 4},  // Node 1 is connected to nodes 0, 3, and 4
            {0},                // Node 2 is connected to node 0
            {1},                // Node 3 is connected to node 1
            {1}                 // Node 4 is connected to node 1
        }
    }
};

static const std::map<std::string, std::vector<std::vector<int>>> unweightedGraphs = [] {
    std::map<std::string, std::vector<std::vector<int>>> combined = undirectedGraphs;
    // Insert directed graphs
    combined.insert(directedGraphs.begin(), directedGraphs.end());
    return combined;
}();

class GraphTest : public ::testing::TestWithParam<
    std::pair<const std::string, 
    std::vector<std::vector<int>>>> 
{};

void testGraphTraversal(const std::vector<std::vector<int>>& adjList, const std::string& graphName, auto& func) {
    // Increase by one each node on which the function is applied
    std::vector<int> result(adjList.size(), 0);
    static auto callback = [&result](int& val) { ++result[val]; };
    func(adjList, 0, callback);
    // Check nodes increment
    if (graphName.find("Disconnected") != std::string::npos) {
        // Node 2 and 3 are not reachable from source 0
        EXPECT_EQ(result, std::vector<int>({1, 1, 0, 0}));
    }
    // Connected graphs
    else {
        for (auto &num : result) EXPECT_EQ(num, 1);
    }
}

TEST_P(GraphTest, ApplyWorks) {
    const auto& [graphName, adjList] = GetParam();
    // BFS
    testGraphTraversal(adjList, graphName, bfsGraphApply);
    // Preorder DFS
    testGraphTraversal(adjList, graphName, preorderGraphApply);
    // Postorder DFS
    testGraphTraversal(adjList, graphName, postorderGraphApply);
}

INSTANTIATE_TEST_SUITE_P(GraphTestsGenerator, GraphTest,
    ::testing::ValuesIn(unweightedGraphs),
    [](const testing::TestParamInfo<GraphTest::ParamType>& info) {
        return info.param.first;
    });

class UndirectedGraphTest : public ::testing::TestWithParam<
    std::pair<const std::string, 
    std::vector<std::vector<int>>>> 
{};

TEST_P(UndirectedGraphTest, IsCyclicWorks) {
    const auto& [graphName, adjList] = GetParam();
    if (graphName.find("Cycle") != std::string::npos || 
        graphName.find("Complete") != std::string::npos) {
        EXPECT_TRUE(isCyclicUndirected(adjList));
    }
    else EXPECT_FALSE(isCyclicUndirected(adjList));
}

INSTANTIATE_TEST_SUITE_P(UndirectedGraphTestsGenerator, UndirectedGraphTest,
    ::testing::ValuesIn(undirectedGraphs),
    [](const testing::TestParamInfo<UndirectedGraphTest::ParamType>& info) {
        return info.param.first;
    });

class DirectedGraphTest : public ::testing::TestWithParam<
    std::pair<const std::string, 
    std::vector<std::vector<int>>>> 
{};

TEST_P(DirectedGraphTest, IsCyclicWorks) {
    const auto& [graphName, adjList] = GetParam();
    if (graphName.find("Cycle") != std::string::npos || 
        graphName.find("Complete") != std::string::npos ||
        graphName.find("Random") != std::string::npos) {
        EXPECT_TRUE(isCyclicDirected(adjList));
    }
    else EXPECT_FALSE(isCyclicDirected(adjList));
}

bool isValidTopologicalOrder(const std::vector<int>& order, const std::vector<std::vector<int>>& adjList) {
    // Map to store the index of each node in the order
    std::unordered_map<int, int> indexMap;
    for (int i = 0; i < order.size(); ++i) indexMap[order[i]] = i;
    for (int node = 0; node < adjList.size(); ++node) {
        for (int neighbor : adjList[node]) {
            // Check if the current node appears before its neighbor in the order
            if (indexMap[node] > indexMap[neighbor]) {
                // Not a valid topological order
                return false;
            }
        }
    }
    return true;
}

TEST_P(DirectedGraphTest, TopSort) {
    const auto& [graphName, adjList] = GetParam();
    // Skip cyclic graphs
    if (graphName.find("Cycle") == std::string::npos &&  
        graphName.find("Complete") == std::string::npos &&
        graphName.find("Random") == std::string::npos) {
        auto result = topologicalSort(adjList);
        EXPECT_TRUE(isValidTopologicalOrder(result, adjList));
    }
}

INSTANTIATE_TEST_SUITE_P(DirectedGraphTestTestsGenerator, DirectedGraphTest,
    ::testing::ValuesIn(directedGraphs),
    [](const testing::TestParamInfo<DirectedGraphTest::ParamType>& info) {
        return info.param.first;
    });

TEST(GraphTest, TreeToGraphWorks) {
    // Test trees
    std::map<std::string, TreeNode<int>*> testTrees = {
        {"EmptyTree", nullptr},
        {"OneElementTree", toBinaryTree({12})},
        {"TwoElementsTree", toBinaryTree({10, 20})},
        {"SimpleTree", toBinaryTree({4, 2, 6, 1, 3, 5, 7})},
    };
    // Expected adjacency lists for each tree
    std::map<std::string, std::unordered_map<int, std::vector<int>>> expectedAdjLists = {
        {"EmptyTree", {}},
        {"OneElementTree", {{12, {}}}},
        {"TwoElementsTree", {{10, {20}}, {20, {10}}}},
        {"SimpleTree", {{4, {2, 6}}, {2, {1, 3, 4}}, {6, {4, 5, 7}}, {1, {2}}, {3, {2}}, {5, {6}}, {7, {6}}}},
    };
    // Loop through each tree and check the conversion
    for (const auto& [treeName, treeRoot] : testTrees) {
        auto adjList = binTreeToGraph(treeRoot);
        // Check that the adjacency list contains the same keys
        EXPECT_EQ(adjList.size(), expectedAdjLists[treeName].size()) << "Failed for tree: " << treeName;
        // Loop through each expected adjacency list entry and verify unordered equality
        for (const auto& [key, expectedNeighbors] : expectedAdjLists[treeName]) {
            // Check if the key exists in the actual adjacency list
            EXPECT_TRUE(adjList.count(key) > 0) << "Key " << key << " not found in adjList for tree: " << treeName;
            // Check for unordered equality of neighbors
            EXPECT_THAT(adjList[key], ::testing::UnorderedElementsAreArray(expectedNeighbors)) 
                << "Failed for tree: " << treeName << ", key: " << key;
        }
    }
    // Clean up the allocated memory
    for (auto& [_, treeRoot] : testTrees) {
        deleteTree(treeRoot);
    }
}

TEST(GraphTest, GetTreeCenterWorks) {
    // Test trees as adjacency lists
    std::map<std::string, std::pair<std::vector<std::vector<int>>, std::vector<int>>> testTrees = {
        {"SingleNode", { {{}}, {0} } },
        {"StraightLine", { {{1}, {0, 2}, {1}, {0}}, {1} } },
        {"StarShape", { {{1, 2, 3}, {0}, {0}, {0}}, {0} } },
        {"TwoNodes", { {{1}, {0}}, {0, 1} } },
        {"SimpleTree", { {{1, 2}, {0, 3}, {0, 4}, {1}, {2}}, {0} } },
        {"BalancedTree", { {{1, 2}, {0, 3, 4}, {0}, {1}, {1}}, {0, 1} } },
    };
    // Loop through each tree and check the center calculation
    for (const auto& [treeName, treeData] : testTrees) {
        const auto& [adjList, expectedCenter] = treeData;
        auto center = getTreeCenter(adjList);
        EXPECT_EQ(center, expectedCenter) << "Failed for tree: " << treeName;
    }
}

TEST(GraphTest, ShortestPathLength) {
    std::vector<std::vector<int>> undirectedGraph = {
        {1, 2},         // 0 -> 1, 2
        {0, 3, 4},  // 1 -> 0, 3, 4
        {0},                // 2 -> 0
        {1},                // 3 -> 1
        {1},                // 4 -> 1
    };
    
    std::vector<std::vector<int>> directedGraph = {
        {1},                // 0 -> 1
        {0, 2, 3},  // 1 -> 0, 2, 3
        {3},                // 2 -> 1
        {1},                // 3 -> 1
        {1}                 // 4 -> 1
    };
    
    // Test shortest paths for graph1
    EXPECT_EQ(shortestPathLen(undirectedGraph, 0, 4), 2);
    EXPECT_EQ(shortestPathLen(undirectedGraph, 0, 3), 2);
    EXPECT_EQ(shortestPathLen(undirectedGraph, 3, 2), 3);
    EXPECT_EQ(shortestPathLen(undirectedGraph, 2, 4), 3);
    EXPECT_EQ(shortestPathLen(undirectedGraph, 2, 2), 0);

    // Test shortest paths for graph2
    EXPECT_EQ(shortestPathLen(directedGraph, 0, 3), 2);
    EXPECT_EQ(shortestPathLen(directedGraph, 2, 3), 1);
    EXPECT_EQ(shortestPathLen(directedGraph, 3, 2), 2);
    EXPECT_EQ(shortestPathLen(directedGraph, 1, 1), 0);
    EXPECT_EQ(shortestPathLen(directedGraph, 4, 1), 1);
    EXPECT_EQ(shortestPathLen(directedGraph, 1, 4), -1);
}

TEST(GraphTest, BfsTreeAndMinPath) {
    std::vector<std::vector<int>> connectedGraph = {
        {1, 2},         // 0 -> 1, 2
        {0, 3},         // 1 -> 0, 3
        {0, 3},         // 2 -> 0, 3
        {1, 2, 4},  // 3 -> 1, 2, 4
        {3}                 // 4 -> 3
    };

    std::vector<std::vector<int>> disconnectedGraph = {
        {1},    // 0 -> 1
        {0},    // 1 -> 0
        {},         // 2 is isolated
        {4},    // 3 -> 4
        {3}     // 4 -> 3
    };

    // Test connected graph
    auto bfsTree = getBfsTree(connectedGraph, 0);
    std::vector<int> expectedTree = {-1, 0, 0, 1, 3};
    EXPECT_EQ(bfsTree, expectedTree);
    // Test path extraction from source 0 to destination 4
    auto pathRec = extractMinPathRec(bfsTree, 0, 4);
    auto pathIter = extractMinPathIter(bfsTree, 0, 4);
    std::vector<int> expectedPath = {0, 1, 3, 4};
    EXPECT_EQ(pathRec, expectedPath);
    EXPECT_EQ(pathIter, expectedPath);
    // Test path extraction from source 0 to destination 2
    pathRec = extractMinPathRec(bfsTree, 0, 2);
    pathIter = extractMinPathIter(bfsTree, 0, 2);
    expectedPath = {0, 2};
    EXPECT_EQ(pathRec, expectedPath);
    EXPECT_EQ(pathIter, expectedPath);

    // Test disconnected graph
    bfsTree = getBfsTree(disconnectedGraph, 0);
    expectedTree = {-1, 0, -1, -1, -1};
    EXPECT_EQ(bfsTree, expectedTree);
    // Test path extraction from source 0 to destination 2 (unreachable)
    pathRec = extractMinPathRec(bfsTree, 0, 2);
    pathIter = extractMinPathIter(bfsTree, 0,  2);
    EXPECT_TRUE(pathRec.empty());
    EXPECT_TRUE(pathIter.empty());
    // Test path extraction from source 0 to destination 4 (unreachable)
    pathRec = extractMinPathRec(bfsTree, 0, 4);
    pathIter = extractMinPathIter(bfsTree, 0, 4);
    EXPECT_TRUE(pathRec.empty());
    EXPECT_TRUE(pathIter.empty());
}
