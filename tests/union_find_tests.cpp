#include <gtest/gtest.h>
#include "union_find.hpp"

using namespace alg;

class UnionFindTest : public ::testing::TestWithParam<int> {};

TEST_P(UnionFindTest, InitializationWorks) {
    int n = GetParam();
    UnionFind uf(n);
    // Each node should be its own parent initially
    for (int i = 0; i != n; ++i) EXPECT_EQ(uf.find(i), i);
}

TEST_P(UnionFindTest, UniteAndFindWork) {
    int n = GetParam();
    UnionFind uf(n);
    // Progressive union of all elements
    for (int i = 0; i < n - 1; ++i) {
        EXPECT_NE(uf.find(i), uf.find(i + 1));
        uf.unite(i, i + 1);
        EXPECT_EQ(uf.find(i), uf.find(i + 1));
    }
    // Validate that all elements are connected to the first element
    for (int i = 1; i < n; ++i) {
        EXPECT_EQ(uf.find(0), uf.find(i));
    }
}

INSTANTIATE_TEST_SUITE_P(
    UnionFindTestsGenerator, 
    UnionFindTest, 
    ::testing::Values(0, 1, 2, 5, 10, 100),
    [](const testing::TestParamInfo<int>& info) {
        return "Size_" + std::to_string(info.param);
    }
);