#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "n_queens.hpp"

namespace alg {
namespace {

using ::testing::TestWithParam;
using ::testing::UnorderedElementsAreArray;
using ::testing::ValuesIn;

using Board = std::vector<std::vector<int>>;

const std::map<std::string, std::pair<int, Board>> testCases = {
    {"OneQueen", {1, {{0}}}},
    {"TwoQueens", {2, {}}},
    {"ThreeQueens", {3,{}}},
    {"FourQueens", {4, {{1, 3, 0, 2}, {2, 0, 3, 1}}}},
    {"FiveQueens", {5, {
    {0, 2, 4, 1, 3}, {0, 3, 1, 4, 2}, {1, 3, 0, 2, 4}, {1, 4, 2, 0, 3}, 
    {2, 0, 3, 1, 4}, {2, 4, 1, 3, 0}, {3, 0, 2, 4, 1}, {3, 1, 4, 2, 0}, 
    {4, 1, 3, 0, 2}, {4, 2, 0, 3, 1}}}},
};

using NQueensTestParam = decltype(testCases)::value_type;

class NQueensTest : public TestWithParam<NQueensTestParam> {};

TEST_P(NQueensTest, WorksWithAllInputs) {
    // Get the parameters for the current test case
    const auto& [n, expected] = GetParam().second;
    // Check if the board is valid
    EXPECT_THAT(nQueens(n), UnorderedElementsAreArray(expected));
}

INSTANTIATE_TEST_SUITE_P(NQueensTestsGenerator, NQueensTest,
    ValuesIn(testCases),
    [](const auto& info) { return info.param.first; });

}  // namespace
}  // namespace alg
