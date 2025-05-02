#include <gtest/gtest.h>
#include <format>
#include "union_find.hpp"

namespace alg {
namespace {

using ::testing::TestWithParam;
using ::testing::Values;

class UnionFindTest : public TestWithParam<int> {};

TEST_P(UnionFindTest, InitializationWorks) {
    const int n = GetParam();
    UnionFind uf(n);
    // Each node should be its own parent initially
    for (int i = 0; i != n; ++i) EXPECT_EQ(uf.find(i), i);
}

TEST_P(UnionFindTest, UniteAndFindWork) {
    const int n = GetParam();
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

INSTANTIATE_TEST_SUITE_P(UnionFindTestsGenerator, UnionFindTest, 
    Values(0, 1, 2, 5, 10, 100),
    [](const auto& info) { return std::format("Size_{}", std::to_string(info.param)); });

}  // namespace
}  // namespace alg